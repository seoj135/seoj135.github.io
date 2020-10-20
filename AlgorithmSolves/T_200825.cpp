// T.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <map>
#include <unordered_map>
#include <array>
#include <deque>
#include <set>
#include <functional>
#include <random>
#include <numeric>
#include <chrono>

std::chrono::system_clock::time_point SWSTART;
#define SWUNIT(duration) std::chrono::duration_cast<std::chrono::microseconds>(duration)
#define SWUNITSTR " ㎲"
#define SW SWSTART = std::chrono::system_clock::now()
#define SWCOUT(prefix) std::cout << #prefix << " : " << SWUNIT(std::chrono::system_clock::now() - SWSTART).count() << SWUNITSTR << std::endl
#define SWGET = SWUNIT(std::chrono::system_clock::now() - SWSTART).count()

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void p1_mov_count_1x1_square() { 
	// L U R D
	// count a 1x1 square
    /* Result: 5
		 ┌┐     ┌┐
	   ┌┤├┐ ┌┤├┐
	   └┼┼┘ └┼┼┘
       ┌┼┼┐ ┌ㅁ┼┐
	   └┘└┘ └┘└┘  ㅁ:start
	*/
	//std::string moves = "UULDRRURDLDDRULLLDRUUUURD";

	/* Result: 7
		 ┌┐     ┌┐
	   ┌┤├┐ ┌┼┼┐
	   └┼┼┘ └┼┼┘
	   ┌┼┼┐ ┌ㅁ┼┐
	   └┘└┘ └┘└┘  ㅁ:start
	*/
	std::string moves = "UULDRRURDLDDRULLLDRUUUURDL";

	struct PT {
        int x, y;

		bool operator<(const PT& other) const
		{
			if (x == other.x) return y < other.y;
			else return x < other.x;
		}
	};

	struct Node {
        PT pt;

		Node* lurd[4] = { nullptr, nullptr, nullptr, nullptr };
	};

	std::map<PT, Node*> map;
	std::map<PT, bool> mapVisits;
	std::map<PT, bool> mapAdded;

	PT current = {0, 0};
    Node start;
	start.pt.x = 0;
	start.pt.y = 0;
	
	map.insert({ current, &start });
	mapVisits.insert({ current, false });
	mapAdded.insert({ current, false });

	int L = 0, U = 1, R = 2, D = 3;

	Node* before = &start;
	Node* cursor;
	// make graph
    for (char c : moves) {
		switch(c) {
		case 'L': current.x++; break;
		case 'U': current.y++; break;
		case 'R': current.x--; break;
		case 'D': current.y--; break;
		}

		auto it = map.find(current);
		if (it == map.end()) {
			cursor = new Node;

			cursor->pt.x = current.x;
			cursor->pt.y = current.y;
			map.insert({ current, cursor });
			mapVisits.insert({ current, false });
			mapAdded.insert({ current, false });
		} else {
			cursor = it->second;
		}

		switch (c) {
		case 'L': before->lurd[L] = cursor; cursor->lurd[R] = before; break;
		case 'U': before->lurd[U] = cursor; cursor->lurd[D] = before; break;
		case 'R': before->lurd[R] = cursor; cursor->lurd[L] = before; break;
		case 'D': before->lurd[D] = cursor; cursor->lurd[U] = before; break;
		}

		before = cursor;
	}
	
	//
	auto checker = [&](Node* cur, int search[4]) {
		Node* cursor = cur;
		for (int i=0; i<4; i++) {
			if (cursor->lurd[search[i]] == nullptr) return 0; // not linked
			
			cursor = cursor->lurd[search[i]];
			
			bool visited = mapVisits.find(cursor->pt)->second;
			if(visited && i < 3) return 0;
		}

		return 1; // found
	};

	//
	int LT[4] = { L, U, R, D };
	int RT[4] = { U, R, D, L };
	int LB[4] = { D, L, U, R };
	int RB[4] = { R, D, L, U };

	// search
	int ans = 0;
	std::deque<Node*> queue;
	queue.push_back(&start);
	while(!queue.empty()) {
		cursor = queue.front();
		queue.pop_front();

		mapVisits.find(cursor->pt)->second = true;
		mapAdded.find(cursor->pt)->second = true;

		int lt = checker(cursor, LT);
		int rt = checker(cursor, RT);
		int lb = checker(cursor, LB);
		int rb = checker(cursor, RB);
		std::cout << "Cur: " << cursor->pt.x << ", " << cursor->pt.y << " : " << lt << " " << rt << " " << lb << " " << rb << std::endl;

		ans += lt + rt + lb + rb;

		for(int go : LT) {
			if(cursor->lurd[go] != nullptr) {
				PT& pt = cursor->lurd[go]->pt;
				if (mapVisits.find(pt)->second == false && mapAdded.find(pt)->second == false) {
					queue.push_back(cursor->lurd[go]);
					mapAdded.find(pt)->second = true;
				}
			}
		}
	}

	std::cout << "Problem 1" << std::endl;
	std::cout << "answer: " << ans << std::endl;
}

void p2_buy_gems(int input, int version = 1) {
	// [3, 7]
	std::vector<std::string> in1 = { "DIA", "RUBY", "RUBY", "DIA", "DIA", "EMERALD", "SAPPHIRE", "DIA" };
	// [1, 3]
	std::vector<std::string> in2 = { "AA", "AB", "AC", "AA", "AC" };
	// [1, 1]
	std::vector<std::string> in3 = { "XYZ", "XYZ", "XYZ" };
	// [1, 5]
	std::vector<std::string> in4 = { "ZZZ", "YYY", "NNNN", "YYY", "BBB" };

	std::vector<std::string> gems = input == 1 ? in1 : input == 2 ? in2 : input == 3 ? in3 : in4;
	std::cout << "Size of in : " << gems.size() << std::endl;
	// find first range of shortest sequential buying to but all of category.

	std::set<std::string> set;
	for (auto& elem : gems) set.insert(elem);

	std::cout << "Set: ";
	for (auto& elem : set) std::cout << elem << " ";
	std::cout << std::endl;

	if (version == 1) {
		auto isIncludeAll = [&](int start, int end) {
			//std::cout << start << " " << end << std::endl;
			//for (auto it = in.begin() + start; it <= in.begin() + end; it++) {
			//	std::cout << *(it) << " ";
			//}
			//std::cout << std::endl;

			for (auto& elem : set) {
				auto it = std::find(gems.begin() + start, gems.begin() + end + 1, elem);
				if (it == gems.begin() + end + 1) return false;
			}
			return true;
		};

		bool found = false;
		for (int LEN = set.size() - 1; LEN < gems.size() && !found; LEN++) {
			for (int i = 0; i < gems.size() - LEN; i++) {
				found = isIncludeAll(i, i + LEN);
				if (found) {
					std::cout << "[" << i + 1 << ", " << i + 1 + LEN << "]" << std::endl;
					break;
				}
			}
		}
	} 
	else if(version == 2) {
		// version 2, try to implement using 2 pointer
		auto isIncludeAll = [&](int start, int end) {
			//std::cout << start << " " << end << std::endl;
			//for (auto it = in.begin() + start; it < in.begin() + end; it++) {
			//	std::cout << *(it) << " ";
			//}
			//std::cout << std::endl;

			if (start == end) return false;

			for (auto& elem : set) {
				auto it = std::find(gems.begin() + start, gems.begin() + end, elem);
				if (it == gems.begin() + end) return false;
			}
			return true;
		};

		int32_t MIN = 1e9;

		// offsets
		int32_t holder[2] = { -1, -1 };
		int a = 0;
		int b = 0;
		bool found = false;
		while(a != gems.size()) {
			found = isIncludeAll(a, b);
			if (!found) {
				if (b < gems.size()) b++;
				else a++;
			}
			else {
				if (b - a < MIN) {
					// hold range
					holder[0] = a + 1;
					holder[1] = b;
					MIN = b - a;
				}
				a++;
			}
		}

		std::cout << "[" << holder[0] << ", " << holder[1] << "]" << std::endl;
	} 
	else {
		// version 3, try to implement using 2 pointer and use map
		std::map<std::string, int32_t> m;
		auto isIncludeAll = [&](int start, int end) {
			if (start == end) return false;

			return m.size() == set.size();
		};

		auto adding = [&](int32_t idx) {
			auto it = m.find(gems[idx]);
			if (it == m.end()) {
				m.insert({ gems[idx], 1 });
			}
			else {
				m[gems[idx]]++;
			}
		};

		auto removing = [&](int32_t idx) {
			m[gems[idx]]--;
			if (m[gems[idx]] == 0) {
				m.erase(gems[idx]);
			}
		};

		int32_t MIN = 1e9;

		// offsets
		int32_t holder[2] = { -1, -1 };
		int a = 0;
		int b = 0;
		bool found = false;
		while (a != gems.size()) {
			found = isIncludeAll(a, b);
			if (!found) {
				if (b < gems.size()) {
					adding(b);
					b++;
				}
				else {
					removing(a);
					a++;
				}
			}
			else {
				if (b - a < MIN) {
					// hold range
					holder[0] = a + 1;
					holder[1] = b;
					MIN = b - a;
				}
				removing(a);
				a++;
			}
		}

		std::cout << "[" << holder[0] << ", " << holder[1] << "]" << std::endl;
	}
}

void p3_3pigs_same_food() {
	std::random_device rd;
	std::mt19937 gen(rd());

	// uniform_int_distribution: [a, b]
	//const int64_t kMIN = INT64_MAX / 128;
	//const int64_t kMAX = INT64_MAX / 32;
	const int64_t kMIN = 10000;
	const int64_t kMAX = 20000;
	std::uniform_int_distribution<int64_t> dist1(kMIN, kMAX);
	std::uniform_int_distribution<int64_t> dist2(0, kMIN / 32 / 3);
	std::normal_distribution<> dist3(3, 1);
	std::uniform_real_distribution<double> dist4(1.0, 2.0);

	int64_t food = dist1(gen);
	food = food - food % 3; // make it multiple of 3
	std::vector<int64_t> foods;
	{
		auto genDiv = [&]() { return std::max((int64_t)dist3(gen), (int64_t)1); };
		
		auto add = [&](int64_t num, std::vector<int64_t>& part) {
			int64_t divide = genDiv();
			for (int32_t i = 0; i < divide; i++) {
				int64_t b = genDiv();
				int64_t t = num / b + num % b;

				part.push_back(t);

				num -= t;
			}
			if (num != 0) part.push_back(num);
		};

		auto addForAPig = [&](int64_t f) {
			std::vector<int64_t> part;

			int64_t foodTemp = f;
			while (foodTemp > 0) {
				// generate
				int64_t temp1 = std::min(dist2(gen), foodTemp);
				foodTemp -= temp1;
				//std::cout << foodTemp << std::endl;

				int64_t positive = (int64_t)(temp1 * dist4(gen));
				int64_t negative = temp1 - positive;
				add(positive, part);
				add(negative, part);
			}
			std::random_shuffle(part.begin(), part.end());
			
			foods.insert(foods.end(), part.begin(), part.end());
		};

		addForAPig(food / 3);
		addForAPig(food / 3);
		addForAPig(food / 3);
	}

	//

	{
		// validation
		std::cout << "Foods Size: " << foods.size() << std::endl;
		int64_t sum = 0;
		for (auto& e : foods) {
			//std::cout << e << " ";
			sum += e;
		}
		std::cout << std::endl;
		std::cout << "food: " << food << std::endl;
		std::cout << "sum: " << sum << std::endl << std::endl;
	}

	//
	// Pigs have to get same food, We can use two divider to divide region
	// So, layout must be like [0 ~~ pig 1] [pig2 ~~ pig2] [pig3 ~~ end]
	// get all of ranges they can get it

	// Use prefix sum
	std::vector<int64_t> prefixSums;
	int64_t prefixSum = 0;
	prefixSums.push_back(0);
	for(auto& e : foods) {
		prefixSum += e;
		prefixSums.push_back(prefixSum);
		//std::cout << prefixSum << std::endl;
	}

	int64_t max = *(prefixSums.end() - 1);
	int64_t maxThird = max / 3;

	std::vector<int32_t> forPig1;
	for(int32_t i = 0; i < foods.size() - 2; i++) {
		int64_t tempSum = prefixSums[i + 1];
		if (tempSum == maxThird) forPig1.push_back(i);			
	}
	std::cout << "max: " << max << ", maxThird: " << maxThird << std::endl;
	std::cout << "viable pig1: " << forPig1.size() << std::endl;

	for(auto& p1 : forPig1) {
		for(int32_t i = p1 + 1; i < foods.size() - 1; i++) {
			int64_t tempSum1 = prefixSums[p1 + 1];
			int64_t tempSum2 = prefixSums[i + 1] - prefixSums[p1 + 1];
			int64_t tempSum3 = prefixSums[foods.size()] - prefixSums[i + 1];

			if(tempSum1 == tempSum2 && tempSum1 == tempSum3) {
				// we found it
				std::cout << "[0, " << p1 << "] [" << p1 << ", " << i << "] [" << i << ", " << foods.size() << "]" << std::endl;
				std::cout << tempSum1 << ", " << tempSum2 << ", " << tempSum3 << std::endl;
			}
		}
	}
}

void p4_segmentation_tree() {
	constexpr int32_t InputLen = 1e6;
	//constexpr int32_t InputLen = 12;

	std::vector<int32_t> input(InputLen);
	std::iota(input.begin(), input.end(), 1);

	// calculate tree size
	int32_t viableHeight = std::ceil(std::log2(InputLen) + 0.5);
	int64_t treeSize = 2 * std::pow(2, viableHeight);

	constexpr int32_t ROOT = 0; // left = node * 2 + 1, right = node * 2 + 2
	std::vector<int64_t> segmentation_tree(treeSize, INT64_MAX);

	// constexpr int32_t ROOT = 1; // left = node * 2, right = node * 2 + 1
	// std::vector<int64_t> segmentation_tree(size, INT64_MAX);

	std::cout << "Tree Height: " << viableHeight << std::endl;
	std::cout << "Tree Size: " << treeSize << std::endl << std::endl;

	auto getMid = [&](int32_t left, int32_t right) {
		return (right + left) / 2;
	};

	//
	// build tree for sum
	std::function<int64_t(int32_t, int32_t, int32_t)> buildTree;
	buildTree = [&](int32_t left, int32_t right, int32_t treeIdx) {
		if(left == right) {
			return segmentation_tree[treeIdx] = input[left];
		}

		int32_t mid = getMid(left, right);
		return segmentation_tree[treeIdx] = 
			buildTree(left, mid, treeIdx * 2 + 1) + 
			buildTree(mid + 1, right, treeIdx * 2 + 2);
	};

	SW;
	buildTree(ROOT, InputLen - 1, ROOT);
	SWCOUT(BUILD_TREE);

	//
	// update
	std::function<void(int32_t, int32_t, int32_t, int32_t, int32_t)> update;
	update = [&](int32_t left, int32_t right, int32_t treeIdx, int32_t idx, int32_t diff) {
		if (left <= idx && idx <= right) {
			segmentation_tree[treeIdx] += diff;

			if (left == right) return;
			int32_t mid = getMid(left, right);
			update(left, mid, treeIdx * 2 + 1, idx, diff);
			update(mid + 1, right, treeIdx * 2 + 2, idx, diff);
		}
	};

	int64_t b = segmentation_tree[0];

	int32_t diff = 100 - input[51];
	SW;
	update(ROOT, InputLen - 1, ROOT, 51, diff);
	SWCOUT(Update_Tree);
	input[51] += diff;
	
	std::cout << "before update: " << b << ", after: " << segmentation_tree[0] << std::endl;
	std::cout << "diff: " << diff << ", elem[51]: " << input[51] << std::endl << std::endl;


	//
	// query
	std::function<int64_t(int32_t, int32_t, int32_t, int32_t, int32_t)> querySumInternal;
	querySumInternal = [&](int32_t left, int32_t right, int32_t treeIdx, int32_t queryLeft, int32_t queryRight) {
		if (queryLeft <= left && right <= queryRight) return segmentation_tree[treeIdx];
		if (queryLeft > right || left > queryRight) {
			// it will be return 0, because I assume the segmentation tree for partial sum
		} else {
			int32_t mid = getMid(left, right);
			return 
				querySumInternal(left, mid, treeIdx * 2 + 1, queryLeft, queryRight) +
				querySumInternal(mid + 1, right, treeIdx * 2 + 2, queryLeft, queryRight);
		}

		return static_cast<int64_t>(0);
	};

	auto querySum = [&](int32_t queryLeft, int32_t queryRight) {
		return querySumInternal(ROOT, InputLen - 1, ROOT, queryLeft, queryRight);
	};

	//
	std::cout << "queries" << std::endl;
	std::cout << querySum(0, 1) << std::endl;
	std::cout << querySum(2, 3) << std::endl;	

	// comparison between naive and segmentation tree
	int32_t left = 50;
	int32_t right = 758259;
	SW;
	std::cout << querySum(left, right) << std::endl;
	SWCOUT(QuerySegtree);

	SW;
	{
		int64_t sum = 0;
		for (int32_t i = left; i <= right; i++) sum += input[i];
		std::cout << sum << std::endl;
	}
	SWCOUT(QueryNaive);
}

void p5_permu_comb() {
	class Generator {
	public:
		std::vector<std::vector<int32_t>> resCombination;
		void combination(std::vector<int32_t>& input, int32_t k, int32_t start, std::vector<int32_t>& current) {
			if (k == current.size()) {
				resCombination.push_back(current);
				return;
			}

			for (; start < input.size(); start++) {
				current.push_back(input[start]);
				combination(input, k, start + 1, current);
				current.pop_back();
			}
		}

		std::vector<std::vector<int32_t>> resPermutation;
		void permutation(std::vector<int32_t>& input, int32_t k, std::vector<int32_t>& current) {
			if (k == current.size()) {
				resPermutation.push_back(current);
				return;
			}

			for (int32_t i = 0; i < input.size(); i++) {
				if (std::find(current.begin(), current.end(), input[i]) != current.end())
					continue;

				current.push_back(input[i]);
				permutation(input, k, current);
				current.pop_back();
			}
		}
	};
	Generator run;

	std::vector<int32_t> INPUT = { 0, 1, 2, 3, 4 };
	std::vector<int32_t> temp;

	int32_t chooser = 3;

	run.combination(INPUT, chooser, 0, temp);

	temp.clear();
	run.permutation(INPUT, chooser, temp);

	auto printer = [](const std::string& title, std::vector<std::vector<int32_t>> input) {
		std::cout << title << std::endl;

		int32_t i = 1;
		for (auto& e : input) {
			std::cout << i++ << " th " << "[ ";
			for (auto& e_ : e) {
				std::cout << e_ << ", ";
			}
			std::cout << " ]" << std::endl;
		}
		std::cout << std::endl;
	};

	class Factorial {
	public:
		int32_t fac(int32_t i) {
			if (i <= 1) return 1;
			return i * fac(i - 1);
		}
		int32_t comb(int32_t n, int32_t k) {
			return fac(n) / fac(n - k) / fac(k);
		}
		int32_t perm(int32_t n, int32_t k) {
			return fac(n) / fac(n - k);
		}
	};
	Factorial fac;

	printer("Combination", run.resCombination);
	std::cout << "count validation: " << fac.comb(INPUT.size(), chooser) << std::endl << std::endl;

	printer("Permutation", run.resPermutation);
	std::cout << "count validation: " << fac.perm(INPUT.size(), chooser) << std::endl << std::endl;
}

#define P(name) std::cout << "#### " << #name << " ####" << std::endl << "# " << std::endl;
#define BR std::cout << std::endl;
#define BR2 BR BR

int main() 
{
	P(P1_mov_count_1x1_square)
	p1_mov_count_1x1_square();
	BR2

	P(P2_buy_gems)
	p2_buy_gems(1);
	p2_buy_gems(2);
	p2_buy_gems(3);
	p2_buy_gems(4);
	BR2

	P(P2_buy_gems_version_2)
	p2_buy_gems(1, 2);
	p2_buy_gems(2, 2);
	p2_buy_gems(3, 2);
	p2_buy_gems(4, 2);
	BR2

	P(P2_buy_gems_version_3)
	p2_buy_gems(1, 3);
	p2_buy_gems(2, 3);
	p2_buy_gems(3, 3);
	p2_buy_gems(4, 3);
	BR2

	P(p3_3pigs_same_food)
	p3_3pigs_same_food();
	BR2

	P(p4_segmentation_tree)
	p4_segmentation_tree();
	BR2

	P(p5_permu_comb)
	p5_permu_comb();
	BR2

	return 0; 
}