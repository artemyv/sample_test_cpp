#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <charconv>
#include <print>

struct PatchInfo
{
	int id;
};
template<typename T>
concept IsFilter = requires (T a, std::string& out, std::string_view in, const PatchInfo& info)
{
	{a.serialize(out)  } -> std::same_as<bool>;
	{a.deserialize(in) } -> std::same_as<bool>;
	{ a.IsMatched(info)} -> std::same_as<bool>;
};

struct IdFilter
{
	std::vector<int> ids;
	bool serialize(std::string& out)
	{
		for(const int i : ids) {
			out += std::format("{};", i);
		}
		return true;
	}
	bool deserialize(std::string_view in)
	{
		try {
			while(!in.empty()) {
				int result{};
				auto [ptr, ec] = std::from_chars(in.data(), in.data() + in.size(), result);
				if(ec == std::errc())
				{
					ids.push_back(result);
					in = in.substr(ptr - in.data() + 1);
				}
				else {
					break;
				}
			}
		}
		catch(...) {
		}
		return in.empty();
	}
	bool IsMatched(const PatchInfo& info)
	{
		return std::find(ids.begin(), ids.end(), info.id) != ids.end();
	}
};

template <typename F> requires IsFilter<F>
struct NotFilter
{
	F m_internal;
	bool serialize(std::string& out) { return m_internal.serilize(out); }
	bool deserialize(std::string_view in) { return m_internal.deserialize(in); }
	bool IsMatched(const PatchInfo& info) { return !m_internal.IsMatched(info); }
};

int main()
{
	std::println("Hello using modules");
	NotFilter<IdFilter> notIds;
	notIds.deserialize("1;2;3;");
	std::println("1 {}, 2 {}, 3 {}, 4 {}", 
		notIds.IsMatched({1}), 
		notIds.IsMatched({2}),
		notIds.IsMatched({3}),
		notIds.IsMatched({4}));

	return 0;
}
