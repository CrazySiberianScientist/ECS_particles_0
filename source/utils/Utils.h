#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>
#include <bitset>

namespace Utils
{
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

	// NOTE: I don't know preferred chunk size and I solved to make it 64bytes as cache line.
	template<typename _Type, size_t _CHUNK_SIZE_BYTES_RATIO = 10, size_t _MIN_CHUNK_SIZE_BYTES = 64>
	class ChunkBuffer
	{
	private:
		static constexpr auto calc_chunk_size_bytes()
		{
			constexpr auto sizeof_type = sizeof(_Type);
			constexpr auto type_chunk_size_bytes = sizeof_type * _CHUNK_SIZE_BYTES_RATIO;
			if constexpr (type_chunk_size_bytes >= _MIN_CHUNK_SIZE_BYTES)
				return type_chunk_size_bytes;
			if constexpr (_MIN_CHUNK_SIZE_BYTES % sizeof_type)
				return (_MIN_CHUNK_SIZE_BYTES / sizeof_type + 1) * sizeof_type;
			return _MIN_CHUNK_SIZE_BYTES;
		}

		static constexpr auto CHUNK_SIZE_BYTES = calc_chunk_size_bytes();
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
			*found_it = constructed_elements.back();
			constructed_elements.pop_back();

			element->~_Type();
			destructed_elements.emplace_back(element);
		}

	private:
		std::vector<Chunk*> chunks;
		std::vector<_Type*> destructed_elements;
		std::vector<_Type*> constructed_elements;
	};

		// NOTE: I don't know preferred chunk size and I solved to make it 64bytes as cache line.
	/*template<typename _Type, size_t _CHUNK_SIZE_BYTES = 64>
	class ChunkTable
	{
	public:
		static constexpr auto CHUNK_SIZE = std::max(_CHUNK_SIZE_BYTES / sizeof(_Type), 1);

	private:
		struct Chunk
		{
			std::bitset<CHUNK_SIZE> valid;
			std::array<_Type, CHUNK_SIZE> data;
		};

	public:
		_Type *get(const size_t key)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = key / CHUNK_SIZE;

			if (chunk_index >= chunks.size() || !(chunk = chunks[chunk_index]))
				return nullptr;

			const auto element_index = key - chunk_index * CHUNK_SIZE;
			if (!chunk->valid[element_index])
				return nullptr;

			return &chunk->data[element_index];
		}

		_Type *get(const size_t key)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = key / CHUNK_SIZE;
			if (!(chunk = chunks[chunk_index]))
			{
				chunk = new Chunk;
				chunks[chunk_index] = chunk;
			}
			else if (chunk_index >= chunks.size())
			{
				chunks.resize(chunk_index + 1);
				chunk = new Chunk;
				chunks.back = chunk;
			}

			return (*chunk)[key - chunk_index * CHUNK_SIZE];
		}


		template<class ...Args>
		_Type* emplace(const size_t key, Args &&...args)
		{
			_Type *current_element = nullptr;
			if (reserved_elements.size())
			{
				current_element = reserved_elements.back();
				reserved_elements.pop_back();
			}
			else
			{
				const auto current_chunk_i = elements_capacity / CHUNK_SIZE;
				Chunk *current_chunk = nullptr;
				if (current_chunk_i >= chunks.size())
					current_chunk = chunks.emplace_back(new Chunk);
				else current_chunk = chunks[current_chunk_i];

				const auto element_index = (elements_capacity++) - current_chunk_i * CHUNK_SIZE;
				current_element = &((*current_chunk)[element_index]);
			}
			++elements_count;
			return new (current_element) _Type(std::forward<Args>(args)...);
		}

		void remove(const size_t key)
		{
			element.~_Type();
			--elements_count;
			reserved_elements.emplace_back(element);
		}

	private:
		std::vector<Chunk*> chunks;
		size_t elements_count = 0;
		size_t elements_capacity = 0;
	};*/
}