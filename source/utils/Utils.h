#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>
#include <bitset>

#define UTILS_ENUM_SEQUENCE(NAME, ...)\
struct NAME{\
	enum {__VA_ARGS__, ENUM_COUNT};\
	using type = std::index_sequence<__VA_ARGS__>;\
}

namespace Utils
{
	template <typename ..._Types>
	struct TypesPack {};

	template <typename ..._Types0, typename ..._Types1>
	constexpr decltype(auto) conCatTypesPack(TypesPack<_Types0...>, TypesPack<_Types1...>) { return TypesPack<_Types0..., _Types1...>{}; }

	template <template <typename> typename _NewType, typename ..._Types0, typename ..._Types1>
	constexpr decltype(auto) combineTypesPack(TypesPack<_Types0...>, TypesPack<_Types1...>) { return _NewType<_Types0..., _Types1...>{}; }

	template <template <typename> typename _NewType, typename ..._Types0>
	constexpr decltype(auto) convertTypesPack(TypesPack<_Types0...>) { return _NewType<_Types0...>{}; }

	template <typename _Type>
	void removeFast(const typename std::vector<_Type>::iterator &it, std::vector<_Type> & container)
	{
		*it = container.back();
		container.pop_back();
	}

	template<size_t _MAX_VALUE>
	class PreferredIntegralType
	{
		using Types = std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>;

		template<size_t _V>
		using PristineType = typename std::decay<decltype(std::get<_V>(Types()))>::type;

		template<size_t _INDEX>
		struct check_limit
		{
			static constexpr auto value =
				_MAX_VALUE >= std::numeric_limits<PristineType<_INDEX - 1>>::max()
				&& _MAX_VALUE <= std::numeric_limits<PristineType<_INDEX>>::max();
		};

		static constexpr size_t value_to_index()
		{
			static_assert(_MAX_VALUE >= 0, "Must be >= 0");
			if constexpr (_MAX_VALUE <= std::numeric_limits<PristineType<0>>::max()) return 0;
			else if constexpr (check_limit<1>::value) return 1;
			else if constexpr (check_limit<2>::value) return 2;
			else if constexpr (check_limit<3>::value) return 3;
		}

	public:
		using type = PristineType<value_to_index()>;
	};

	// NOTE: Min chunk size is 64bytes as cache line.
	template<typename _Type, size_t _CHUNK_SIZE_BYTES_RATIO = 10, size_t _MIN_CHUNK_SIZE_BYTES = 64>
	static constexpr auto calcPreferredChunkSizeBytes()
	{
		constexpr auto sizeof_type = sizeof(_Type);
		constexpr auto type_chunk_size_bytes = sizeof_type * _CHUNK_SIZE_BYTES_RATIO;
		if constexpr (type_chunk_size_bytes >= _MIN_CHUNK_SIZE_BYTES)
			return type_chunk_size_bytes;
		if constexpr ((_MIN_CHUNK_SIZE_BYTES % sizeof_type) != 0)
			return (_MIN_CHUNK_SIZE_BYTES / sizeof_type + 1) * sizeof_type;
		return _MIN_CHUNK_SIZE_BYTES;
	}

	template<typename _Type, size_t _CHUNK_SIZE_BYTES_RATIO = 10, size_t _MIN_CHUNK_SIZE_BYTES = 64>
	class ChunkBuffer
	{
	private:
		static constexpr auto CHUNK_SIZE_BYTES = calcPreferredChunkSizeBytes<_Type, _CHUNK_SIZE_BYTES_RATIO, _MIN_CHUNK_SIZE_BYTES>();
		static constexpr auto CHUNK_SIZE = CHUNK_SIZE_BYTES / sizeof(_Type);

		using Chunk = std::array<uint8_t, CHUNK_SIZE_BYTES>;

	public:
		~ChunkBuffer()
		{
			for (auto &element : constructed_elements)
				element->~_Type();
			for (auto &chunk : chunks)
				delete chunk;
		}

		template<class ...Args>
		_Type* emplace_back(Args &&...args)
		{
			_Type *current_element = nullptr;
			if (destructed_elements.size())
			{
				current_element = destructed_elements.back();
				destructed_elements.pop_back();
			}
			else
			{
				const auto elements_count = constructed_elements.size();
				const auto current_chunk_i = elements_count / CHUNK_SIZE;
				Chunk *current_chunk = nullptr;
				if (current_chunk_i >= chunks.size())
					current_chunk = chunks.emplace_back(new Chunk);
				else current_chunk = chunks[current_chunk_i];

				const auto element_index = (elements_count - current_chunk_i * CHUNK_SIZE) * sizeof(_Type);
				current_element = reinterpret_cast<_Type*>(&((*current_chunk)[element_index]));
			}
			new (current_element) _Type(std::forward<Args>(args)...);
			constructed_elements.emplace_back(current_element);
			
			return current_element;
		}

		void remove(_Type *element)
		{
			auto found_it = std::find(constructed_elements.begin(), constructed_elements.end(), element);
			if (found_it == constructed_elements.end()) return;
			Utils::removeFast(found_it, constructed_elements);

			element->~_Type();
			destructed_elements.emplace_back(element);
		}

	private:
		std::vector<Chunk*> chunks;
		std::vector<_Type*> destructed_elements;
		std::vector<_Type*> constructed_elements;
	};

	template<typename _Type, size_t _CHUNK_SIZE_BYTES_RATIO = 10, size_t _MIN_CHUNK_SIZE_BYTES = 64>
	class ChunkTable
	{
	private:
		static constexpr auto CHUNK_SIZE_BYTES = calcPreferredChunkSizeBytes<_Type, _CHUNK_SIZE_BYTES_RATIO, _MIN_CHUNK_SIZE_BYTES>();
		static constexpr auto CHUNK_SIZE = CHUNK_SIZE_BYTES / sizeof(_Type);

		struct Chunk
		{
			std::bitset<CHUNK_SIZE> valid;
			std::array<uint8_t, CHUNK_SIZE_BYTES> data;
		};

	public:
		~ChunkTable()
		{
			for (auto &chunk : chunks)
			{
				if (!chunk) continue;
				for (auto i = 0; i < CHUNK_SIZE; ++i)
				{
					if (!chunk->valid[i]) continue;
					reinterpret_cast<_Type*>(&chunk->data[i * sizeof(_Type)])->~_Type();
				}
				delete chunk;
			}
		}

		_Type *get(const size_t key)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = key / CHUNK_SIZE;

			if (chunk_index >= chunks.size() || !(chunk = chunks[chunk_index]))
				return nullptr;

			const auto element_index = key - chunk_index * CHUNK_SIZE;
			if (!chunk->valid[element_index])
				return nullptr;

			return reinterpret_cast<_Type*>(&chunk->data[element_index * sizeof(_Type)]);
		}

		template<class ...Args>
		_Type* emplace(const size_t key, Args &&...args)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = key / CHUNK_SIZE;
			if (chunk_index >= chunks.size())
			{
				chunks.resize(chunk_index + 1);
				chunk = new Chunk;
				chunks.back() = chunk;
			}
			else if (!(chunk = chunks[chunk_index]))
			{
				chunk = new Chunk;
				chunks[chunk_index] = chunk;
			}

			const auto element_index = key - chunk_index * CHUNK_SIZE;
			auto current_element = reinterpret_cast<_Type*>(&(chunk->data[element_index * sizeof(_Type)]));
			if (chunk->valid[element_index])
				return current_element;

			new (current_element) _Type(std::forward<Args>(args)...);
			chunk->valid[element_index] = true;

			return current_element;
		}

		void remove(const size_t key)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = key / CHUNK_SIZE;

			if (chunk_index >= chunks.size() || !(chunk = chunks[chunk_index]))
				return;

			const auto element_index = key - chunk_index * CHUNK_SIZE;
			if (!chunk->valid[element_index])
				return;

			chunk->valid[element_index] = false;
			auto element = reinterpret_cast<_Type*>(&chunk->data[element_index * sizeof(_Type)]);
			element->~_Type();
		}

	private:
		std::vector<Chunk*> chunks;
	};
}