#pragma once

#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>
#include <array>

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

	// NOTE: I don't know preferred chunk size and I have solved to make it 64bytes as cache line.
	template<typename _Type, size_t _CHUNK_SIZE_BYTES = 64>
	class ChunkVector
	{
	public:
		_Type &getSparse(size_t index)
		{
			Chunk* chunk = nullptr;
			const auto chunk_index = index / CHUNK_SIZE;
			if (chunk_index >= chunks.size())
			{
				chunks.resize(chunk_index + 1);
				chunk = new Chunk;
				chunks.back = chunk;
			}
			else if (!(chunk = chunks[chunk_index]))
			{
				chunk = new Chunk;
				chunks[chunk_index] = chunk;
			}
			
			return (*chunk)[index - chunk_index * CHUNK_SIZE];
		}

		_Type &getContiguous(size_t index)
		{
			const auto chunk_index = index / CHUNK_SIZE;
			return (*chunk)[index - chunk_index * CHUNK_SIZE];
		}

		template<class ...Args>
		_Type &emplace_back(Args &&...args)
		{
			const auto current_chunk_i = elements_count / CHUNK_SIZE;
			Chunk *current_chunk = nullptr;
			if (current_chunk_i >= chunks.size())
				current_chunk = chunks.emplace_back(new Chunk);
			else current_chunk = chunks[current_chunk_i];

			const auto element_index = (elements_count++) - current_chunk_i * CHUNK_SIZE;
			auto element = new (&(*current_chunk)[element_index]) _Type(std::forward<Args>(args)...);
			return *element;
		}

	private:
		static constexpr auto CHUNK_SIZE = std::max(_CHUNK_SIZE_BYTES / sizeof(_Type), 1);

		using Chunk = std::array<_Type, CHUNK_SIZE>;
		std::vector<Chunk*> chunks;
		size_t elements_count = 0;
	};
}