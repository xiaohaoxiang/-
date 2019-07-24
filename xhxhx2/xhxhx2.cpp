#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <list>


// 成绩管理系统命名空间
namespace xhx
{

	// -----------------前置声明区-----------------

	class Object;
	class Student;
	class Exam;
	class School;



	// -----------------类型别名定义区-----------------

	using ull                          = unsigned long long;
	using mark_type                    = double;
	using rank_type                    = size_t;
	using numb_type                    = ull;
	using name_type                    = std::string;
	using weight_type                  = double;
	using stu_list                     = std::list<Student>;
	using exam_list                    = std::list<Exam>;
	using stu_iterator                 = stu_list::iterator;
	using const_stu_it                 = stu_list::const_iterator;
	using exam_iterator                = exam_list::iterator;
	using const_exam_it                = exam_list::const_iterator;
	using stu_it_vector                = std::vector<stu_iterator>;
	using exam_it_vector               = std::vector<exam_iterator>;
	using const_stu_it_vec             = std::vector<const_stu_it>;
	using const_exam_it_vec            = std::vector<const_exam_it>;
	using mark_rank                    = std::pair<mark_type, rank_type>;
	using mark_rank_vec                = std::vector<mark_rank>;
	using const_stu_it_mark            = std::pair<const_stu_it, mark_type>;
	using const_stu_it_mark_vec        = std::vector<const_stu_it_mark>;
	using const_stu_it_rank            = std::pair<const_stu_it, rank_type>;
	using const_stu_it_rank_vec        = std::vector<const_stu_it_rank>;
	using const_exam_it_mark_rank      = std::pair<const_exam_it, mark_rank>;
	using const_exam_it_mark_rank_vec  = std::vector<const_exam_it_mark_rank>;
	using name_numb                    = Object;



	// -----------------常量定义区-----------------

	enum Impr_Mark :signed int
	{
		Mark_Total = 100,
		Mark_Pass = 60,
		Mark_Zero = 0,
		Mark_Invalid = -1
	};
	const char* const _Blanks = "      ";



	// -----------------声明区-----------------

	// 重载比较函数集合, 以 Numb() 作为关键字
	struct less_Numb
	{
		template<typename ItL, typename ItR>
		inline bool operator() (const ItL& _Left, const ItR& _Right) const noexcept;

		template<typename First1, typename Second1, typename First2, typename Second2>
		inline bool operator() (const std::pair<First1, Second1>& _Left, const std::pair<First2, Second2>& _Right) const noexcept;

		template<typename ItR>
		inline bool operator() (const numb_type& _Left, const ItR& _Right) const noexcept;

		template<typename ItL>
		inline bool operator() (const ItL& _Left, const numb_type& _Right) const noexcept;

		inline bool operator() (const Object& _Left, const Object& _Right) const noexcept;

		inline bool operator() (const numb_type& _Left, const numb_type& _Right) const noexcept;
	};

	// 重载比较函数集合, 以 Name() 作为第一关键字, Numb() 作为第二关键字
	struct less_Name
	{
		template<typename ItL, typename ItR>
		inline bool operator() (const ItL& _Left, const ItR& _Right) const noexcept;

		template<typename First1, typename Second1, typename First2, typename Second2>
		inline bool operator() (const std::pair<First1, Second1>& _Left, const std::pair<First2, Second2>& _Right) const noexcept;

		template<typename ItR>
		inline bool operator() (const name_type& _Left, const ItR& _Right) const noexcept;

		template<typename ItL>
		inline bool operator() (const ItL& _Left, const name_type& _Right) const noexcept;

		inline bool operator() (const Object& _Left, const Object& _Right) const noexcept;

		inline bool operator() (const name_type& _Left, const name_type& _Right) const noexcept;
	};


	// Object类, 提供编号和名称接口
	class Object
	{
	public:
		Object(const numb_type& _Numb = 0, const name_type & _Name = name_type());
		Object(const name_type& _Name, const numb_type& _Numb = 0);
		Object(const Object&) = default;
		~Object() = default;
		Object& operator= (const Object&) = default;

		const inline name_type& Name() const noexcept;
		const inline numb_type& Numb() const noexcept;

	protected:

		numb_type _Numb;
		name_type _Name;

	};


	// Student类, 公有继承Object类
	class Student :public Object
	{
	public:
		friend class School;

		// Student::排序方式枚举
		enum SortedType :signed int
		{
			Sorted_Chronologically = 11,                         // 按时间顺序排序
			Sorted_Chronologically_Reverse = 12,                 // 按时间逆序排序
			Sorted_by_Numb_Non_Descending = 13,                  // 按考试编号非降序排序
			Sorted_by_Numb_Non_Ascending = 14,                   // 按考试编号非升序排序
			Sorted_by_Name_Non_Descending = 15,                  // 按考试名称字典序非降序排序
			Sorted_by_Name_Non_Ascending = 16,                   // 按考试名称字典序非升序排序
			Sorted_by_Mark_Non_Descending = 17,                  // 按分数非降序排序
			Sorted_by_Mark_Non_Ascending = 18,                   // 按分数非升序排序
			Sorted_by_Weight_Non_Descending = 19,                // 按学分非降序排序
			Sorted_by_Weight_Non_Ascending = 20                  // 按学分非升序排序
		};

	public:

		Student(const numb_type& _Numb, const name_type& _Name);
		Student();
		Student(const Student&) = default;
		~Student() = default;
		Student& operator= (const Student&) = default;

		// 返回当前学生的总学分
		inline const weight_type& Credit() const noexcept;

		// 返回当前学生的加权成绩
		inline mark_type Weighted_Mark() const noexcept;

		// 返回当前学生参加过的考试的迭代器向量, 以 _Type 方式排序
		const_exam_it_vec Vec_const_exam_it(SortedType _Type = Sorted_Chronologically) const noexcept;

		// 返回当前学生参加过的考试的 pair<const_stu_it, pair<mark_type, rank_type>>向量, 以 _Type 方式排序
		const_exam_it_mark_rank_vec Vec_const_exam_it_mark_rank(SortedType _Type = Sorted_Chronologically) const noexcept;

		// 输出当前学生参加过的考试的成绩情况至 out, 以 _Type 方式排序
		void output(std::ostream& out, SortedType _Type = Sorted_Chronologically) const;

		friend std::ostream& operator<< (std::ostream& out, const Student& stu);

	private:

		const_exam_it_vec _Exams;                                // 当前学生参加过的考试的迭代器向量
		mark_type _Weighted_Total_Mark;                          // 当前学生的 权重*分数 总和
		weight_type _Credit;                                     // 当前学生的学分总和

		// 为当前学生添加一场考试
		void _add_exam(const const_exam_it& _Exam, const mark_type& _My_Mark) noexcept;

		// 输出单次考试成绩
		static void _output(std::ostream& out, const const_exam_it& _Exam_CIt, const mark_rank& _Mark_Rank, const size_t& _Order = 0);
	};


	// Exam类, 公有继承Object类
	class Exam :public Object
	{
	public:
		friend class School;

		// Exam::排序方式枚举
		enum SortedType :signed int
		{
			Sorted_by_Numb_Non_Descending = 21,                  // 按学号非降序排序
			Sorted_by_Numb_Non_Ascending = 22,                   // 按学号非升序排序
			Sorted_by_Name_Non_Descending = 23,                  // 按姓名字典序非降序排序
			Sorted_by_Name_Non_Ascending = 24,                   // 按姓名字典序非升序排序
			Sorted_by_Mark_Non_Descending = 25,                  // 按分数非降序排序 (分数相同按学号升序排序)
			Sorted_by_Mark_Non_Ascending = 26                    // 按分数非升序排序 (分数相同按学号降序排序)
		};

	private:

		// 用来分析结果的聚合类, 存储考试分析
		struct _Analyzer
		{
			mark_type mean;                                      // 平均分
			mark_type highest;                                   // 最高分
			mark_type lowest;                                    // 最低分
			double standard_deviation;                           // 标准差

			_Analyzer(Exam& _Exam);
			~_Analyzer() = default;
			_Analyzer(const _Analyzer&) = default;
			_Analyzer() = delete;
			_Analyzer& operator= (const _Analyzer&) = delete;
		};
		friend struct _Analyzer;

	public:

		Exam(const School& _Sch, const const_stu_it_mark_vec& _Vec, const numb_type& _Numb, const name_type& _Name = name_type(), weight_type _Weight = 0);
		~Exam() = default;
		Exam(const Exam&) = default;
		Exam() = delete;
		Exam& operator= (const Exam&) = delete;

		// 返回当前考试的学生数量
		const size_t& Count() const noexcept;

		// 返回当前考试的权值
		const weight_type& Weight() const noexcept;

		// 返回当前考试所在School的常引用
		const School& in_School() const noexcept;


		// 以学号作为关键字查找, 返回第一个Numb()不小于查找学号的学生迭代器, 若不存在则返回学号最大的学生迭代器
		const_stu_it find_stu(const numb_type& Stu_Numb) const noexcept;

		// 以姓名作为关键字查找, 返回所有姓名等于查找姓名的学生迭代器向量
		const_stu_it_vec find_stu(const name_type& Stu_Name) const noexcept;

		// 以学号作为关键字查找, 返回第一个Numb()不小于查找学号的学生的分数, 不存在则返回Impr_Mark::Mark_Invalid
		mark_type get_mark(const numb_type& Stu_Numb) const noexcept;

		// 以学号作为关键字查找, 返回第一个Numb()不小于查找学号的学生的 mark_rank = pair<mark_type, rank_type>, 不存在则返回 pair(Impr_Mark::Mark_Invalid, 0)
		mark_rank get_mark_rank(const numb_type& Stu_Numb) const noexcept;

		// 以学号作为关键字查找, 返回第一个Numb()不小于查找学号的学生的排名, 不存在则返回0
		rank_type get_rank(const numb_type& Stu_Numb) const noexcept;

		// 以分数作为关键字查找, 返回第一个不大于查找分数的排名
		rank_type get_rank2(const mark_type& _Mark) const noexcept;

		// 返回当前考试所有学生的 pair<const_stu_it, mark_type>向量, 以 _Type 方式排序
		const_stu_it_mark_vec Vec_const_stu_it_mark(SortedType _Type) const noexcept;

		// 返回当前考试所有学生的迭代器向量, 以 _Type 方式排序
		const_stu_it_vec Vec_const_stu_it(SortedType _Type) const noexcept;

		// 输出当前考试所有学生的成绩信息至 out, 以 _Type 方式排序
		void output(std::ostream& out, SortedType _Type = Sorted_by_Mark_Non_Ascending) const;

		friend std::ostream& operator<< (std::ostream& out, const Exam& _Exam);
		friend std::ostream& operator<< (std::ostream& out, const _Analyzer& anal);

	private:

		const School& _Sch;                                      // 当前考试所在School的常引用
		const weight_type _Weight;                               // 当前考试的权值
		const size_t _Count;                                     // 当前考试的学生数量

		const_stu_it_mark_vec _Stu_It_Mark_Vec;                  // 当前考试的 pair<const_stu_it, mark_type>向量, 初始化后按less_Numb排序
		const_stu_it_vec _Stu_It_Vec_Name;                       // 当前考试的学生迭代器向量, 初始化后按less_Name排序
		mark_rank_vec _Mark_Table;                               // 当前考试的一分一段表, 按分数降序排序
		const _Analyzer _Anal;                                   // 当前考试的考试分析对象

		// 重载输出单个学生的成绩信息至 out
		static void _output(std::ostream& out, const numb_type& _Stu_Numb, const name_type& _Stu_Name, const mark_type& _Mark, const rank_type& _Rank, const size_t& _Order = 0);

		// 重载输出单个学生的成绩信息至 out
		static void _output(std::ostream& out, const const_stu_it& _Stu_CIt, const mark_type& _Mark, const rank_type& _Rank, const size_t& _Order = 0);

		// 输出 [_Begin, _End)区间 的学生的成绩信息至 out
		template<typename RanIt>
		void _output_range(std::ostream& out, RanIt _Begin, RanIt _End) const;
	};

	class School :public Object
	{
	public:

		// School::排序方式枚举
		enum SortedType :signed int
		{
			Sorted_by_Numb_Non_Descending = 31,                  // 按编号非降序排序
			Sorted_by_Numb_Non_Ascending = 32,                   // 按编号非升序排序
			Sorted_by_Name_Non_Descending = 33,                  // 按名称字典序非降序排序
			Sorted_by_Name_Non_Ascending = 34,                   // 按名称字典序非升序排序
			Sorted_by_Mark_Non_Descending = 35,                  // 按分数非降序排序 (分数相同按编号升序排序)
			Sorted_by_Mark_Non_Ascending = 36,                   // 按分数非升序排序 (分数相同按编号降序排序)
			Sorted_by_Weight_Non_Descending = 37,                // 按学分/权重非降序排序
			Sorted_by_Weight_Non_Ascending = 38                  // 按学分/权重非升序排序
		};

	public:

		School(const numb_type& School_Numb = 0, const name_type & School_Name = name_type());
		~School() = default;

		// 返回存有所有学生的链表的常引用, 是Student对象的存储容器
		const stu_list& Stu_List() const noexcept;

		// 返回存有所有考试的链表的常引用, 是Exam对象的存储容器
		const exam_list& Exam_List() const noexcept;

		// 以学号作为关键字查找学生, 返回第一个Numb()不小于查找学号的学生迭代器, 若不存在则返回Stu_List().cend()
		const_stu_it find_stu(const numb_type& Stu_Numb) const noexcept;

		// 以姓名作为关键字查找学生, 返回所有Name()等于查找姓名的学生迭代器向量
		const_stu_it_vec find_stu(const name_type& Stu_Name) const noexcept;

		// 以编号作为关键字查找考试, 返回第一个Numb()不小于查找编号的考试迭代器, 若不存在则返回Exam_List().cend()
		const_exam_it find_exam(const numb_type& Exam_Numb) const noexcept;

		// 以名称作为关键字查找考试, 返回所有Name()等于查找名称的考试迭代器向量
		const_exam_it_vec find_exam(const name_type& Exam_Name) const noexcept;

		// 添加一次考试信息, 添加成功返回该考试的迭代器, 否则返回Exam_List().cend(), 从 in 读取数据, 以 Exam_Numb 作为考试编号, Exam_Name 作为考试名称, _Weight 作为权值
		const_exam_it hold_exam(std::istream& in, const numb_type& Exam_Numb, const name_type& Exam_Name, const weight_type& _Weight);

		// 返回所有学生的迭代器向量, 以 _Type 方式排序
		const_stu_it_vec Vec_const_stu_it(SortedType _Type = Sorted_by_Numb_Non_Descending) const noexcept;

		// 返回所有考试的迭代器向量, 以 _Type 方式排序
		const_exam_it_vec Vec_const_exam_it(SortedType _Type = Sorted_by_Numb_Non_Descending) const noexcept;

		// 返回所有学生的 pair<const_stu_it, rank>向量, 以 Sorted_by_Mark_Non_Ascending(分数非升序)排序
		const_stu_it_rank_vec Vec_const_stu_it_rank() const noexcept;

		// 返回所有学生的 pair<const_stu_it, rank>向量, 以 _Type 方式排序
		const_stu_it_rank_vec Vec_const_stu_it_rank(SortedType _Type) const noexcept;

		// 输出所有学生的考试与成绩信息至 out, 以 _Type 方式排序
		void output_stu(std::ostream& out, SortedType _Type = Sorted_by_Numb_Non_Descending) const;

		// 输出所有考试的考试情况至 out, 以 _Type 方式排序
		void output_exam(std::ostream& out, SortedType _Type = Sorted_by_Numb_Non_Descending) const;

	private:

		stu_list _Stu_List;                                      // 存有所有学生的链表, 是Student对象的存储容器
		exam_list _Exam_List;                                    //存有所有考试的链表, 是Exam对象的存储容器

		// 以学生的 Numb() 作为关键字的 pair<numb_type, stu_iterator>关联容器
		std::map<numb_type, stu_iterator, less_Numb> _Map_Stu_numb;

		// 以学生的 Name() 作为第一关键字, Numb() 作为第二关键字的 pair<pair<name_type, numb_type>, stu_iterator>关联容器
		std::map<name_numb, stu_iterator, less_Name> _Map_Stu_name_numb;

		// 以考试的 Numb() 作为关键字的 pair<numb_type, exam_iterator>关联容器
		std::map<numb_type, exam_iterator, less_Numb> _Map_Exam_numb;

		// 以考试的 Name() 作为第一关键字, Numb() 作为第二关键字的 pair<pair<name_type, numb_type>, exam_iterator>关联容器
		std::map<name_numb, exam_iterator, less_Name> _Map_Exam_name_numb;

		// 以学号作为关键字查找学生, 返回第一个Numb()不小于查找学号的学生迭代器, 若不存在则返回Stu_List().end()
		stu_iterator _find_stu(const numb_type& Stu_Numb) noexcept;

		// 以姓名作为关键字查找学生, 返回所有Name()等于查找姓名的学生迭代器向量
		stu_it_vector _find_stu(const name_type& Stu_Name) noexcept;

		// 以编号作为关键字查找考试, 返回第一个Numb()不小于查找编号的考试迭代器, 若不存在则返回Exam_List().end()
		exam_iterator _find_exam(const numb_type& Exam_Numb) noexcept;

		// 以名称作为关键字查找考试, 返回所有Name()等于查找名称的考试迭代器向量
		exam_it_vector _find_exam(const name_type& Exam_Name) noexcept;

		// 不检查地构造一个学生对象至 _Stu_List 末尾, 返回指向该对象的迭代器
		stu_iterator _add_stu_unchecked(const numb_type& Stu_Numb, const name_type& Stu_Name) noexcept;

		// 不检查地构造一次考试对象至 _Exam_List 末尾, 返回指向该对象的迭代器
		exam_iterator _add_exam_unchecked(const const_stu_it_mark_vec& _Vec, const numb_type& Exam_Numb, const name_type& Exam_Name, const weight_type& _Weight) noexcept;

		// 检查学生学号是否尚未被占用, 未被占用返回 true, 否则返回 false
		inline bool _Check_Stu_Numb(const numb_type& Stu_Numb) const noexcept;

		// 检查考试编号是否尚未被占用, 未被占用返回 true, 否则返回 false
		inline bool _Check_Exam_Numb(const numb_type& Exam_Numb) const noexcept;

		// 检查 _src 是否是合法的成绩输入流
		inline bool _Check_Istream(std::istream& _src);

		// 输出单个学生的考试成绩信息至 out
		static void _output_stu(std::ostream& out, const const_stu_it& _Stu_CIt, const rank_type& _Rank = 0, const size_t & _Order = 0);

		// 输出单次考试的考试情况至 out
		static void _output_exam(std::ostream& out, const const_exam_it& _Exam_CIt, const size_t& _Order = 0);

	};





	// -----------------定义与实现区-----------------

	Student::Student(const numb_type& _Numb, const name_type& _Name)
		: Object(_Numb, _Name), _Weighted_Total_Mark(0), _Credit(0) {}

	Student::Student()
		: Object(), _Weighted_Total_Mark(0), _Credit(0) {}

	inline const weight_type& Student::Credit() const noexcept
	{
		return _Credit;
	}

	inline mark_type Student::Weighted_Mark() const noexcept
	{
		return _Weighted_Total_Mark / _Credit;
	}

	const_exam_it_vec Student::Vec_const_exam_it(SortedType _Type) const noexcept
	{
		const_exam_it_vec mVec;

		switch (_Type)
		{
		case xhx::Student::Sorted_Chronologically:
			//nothing to do
			break;
		case xhx::Student::Sorted_Chronologically_Reverse:
			mVec = _Exams;
			std::reverse(mVec.begin(), mVec.end());
			break;
		case xhx::Student::Sorted_by_Numb_Non_Descending:
			mVec = _Exams;
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::Student::Sorted_by_Numb_Non_Ascending:
			mVec = _Exams;
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::Student::Sorted_by_Name_Non_Descending:
			mVec = _Exams;
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::Student::Sorted_by_Name_Non_Ascending:
			mVec = _Exams;
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::Student::Sorted_by_Weight_Non_Descending:
			mVec = _Exams;
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Left->Weight() < _Right->Weight();
				}
			);
			break;
		case xhx::Student::Sorted_by_Weight_Non_Ascending:
			mVec = _Exams;
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Right->Weight() < _Left->Weight();
				}
			);
			break;
		default:
			auto&& _Vec_mark = Vec_const_exam_it_mark_rank(_Type);
			for (auto&& _It = _Vec_mark.begin(); _It != _Vec_mark.end(); ++_It)
			{
				mVec.push_back(_It->first);
			}
			break;
		}
		return mVec;
	}

	const_exam_it_mark_rank_vec Student::Vec_const_exam_it_mark_rank(SortedType _Type) const noexcept
	{
		const_exam_it_mark_rank_vec mVec;
		for (auto&& _It1 = _Exams.begin(); _It1 != _Exams.end(); ++_It1)
		{
			const auto& _ITto = *_It1;
			mVec.emplace_back(_ITto, _ITto->get_mark_rank(_Numb));
		}

		switch (_Type)
		{
		case xhx::Student::Sorted_Chronologically:
			// nothing to do
			break;
		case xhx::Student::Sorted_Chronologically_Reverse:
			std::reverse(mVec.begin(), mVec.end());
			break;
		case xhx::Student::Sorted_by_Numb_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::Student::Sorted_by_Numb_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::Student::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::Student::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::Student::Sorted_by_Mark_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it_mark_rank& _Left, const const_exam_it_mark_rank& _Right)->bool
				{
					return _Left.second.first < _Right.second.first;
				}
			);
			break;
		case xhx::Student::Sorted_by_Mark_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it_mark_rank& _Left, const const_exam_it_mark_rank& _Right)->bool
				{
					return _Right.second.first < _Left.second.first;
				}
			);
			break;
		case xhx::Student::Sorted_by_Weight_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it_mark_rank& _Left, const const_exam_it_mark_rank& _Right)->bool
				{
					return _Left.first->Weight() < _Right.first->Weight();
				}
			);
			break;
		case xhx::Student::Sorted_by_Weight_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it_mark_rank& _Left, const const_exam_it_mark_rank& _Right)->bool
				{
					return _Right.first->Weight() < _Left.first->Weight();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}

	void Student::output(std::ostream& out, SortedType _Type) const
	{
		const_exam_it_mark_rank_vec mVec(Vec_const_exam_it_mark_rank(_Type));

		out << "   考试编号 考试名称  分数   排名\n";
		auto&& _Begin = mVec.begin();
		for (size_t _cnt = 1; _Begin != mVec.end(); ++_Begin, ++_cnt)
		{
			_output(out, _Begin->first, _Begin->second, _cnt);
		}
	}

	void Student::_add_exam(const const_exam_it& _Exam, const mark_type& _My_Mark) noexcept
	{
		_Exams.push_back(_Exam);
		_Weighted_Total_Mark += _Exam->Weight() * _My_Mark;
		_Credit += _Exam->Weight();
	}

	void Student::_output(std::ostream& out, const const_exam_it& _Exam_CIt, const mark_rank& _Mark_Rank, const size_t& _Order)
	{
		const auto& _Ex = *_Exam_CIt;
		if (_Order)
		{
			out << _Order << _Blanks;
		}
		out << _Ex.Numb() << _Blanks << _Ex.Name() << _Blanks << _Mark_Rank.first << _Blanks << _Mark_Rank.second << '\n';
	}

	School::School(const numb_type& School_Numb, const name_type& School_Name)
		: Object(School_Numb, School_Name) {}

	const stu_list& School::Stu_List() const noexcept
	{
		return _Stu_List;
	}

	const exam_list& School::Exam_List() const noexcept
	{
		return _Exam_List;
	}

	const_stu_it School::find_stu(const numb_type& Stu_Numb) const noexcept
	{
		auto&& _pos = _Map_Stu_numb.find(Stu_Numb);
		return _pos == _Map_Stu_numb.cend() ? _Stu_List.cend() : const_stu_it(_pos->second);
	}

	const_stu_it_vec School::find_stu(const name_type& Stu_Name) const noexcept
	{
		const_stu_it_vec v;
		for (auto&& first = _Map_Stu_name_numb.lower_bound(Stu_Name); first != _Map_Stu_name_numb.end() && first->second->Name() == Stu_Name; ++first)
		{                                      // 此处发生了隐式类型转换:string -> Object
			v.push_back(first->second);
		}
		return v;
	}

	const_exam_it School::find_exam(const numb_type& Exam_Numb) const noexcept
	{
		auto&& _pos = _Map_Exam_numb.find(Exam_Numb);
		return _pos == _Map_Exam_numb.cend() ? _Exam_List.cend() : const_exam_it(_pos->second);
	}

	const_exam_it_vec School::find_exam(const name_type& Exam_Name) const noexcept
	{
		const_exam_it_vec v;
		for (auto&& first = _Map_Exam_name_numb.lower_bound(Exam_Name); first != _Map_Exam_name_numb.end() && first->second->Name() == Exam_Name; ++first)
		{                                      // 此处发生了隐式类型转换:string -> Object
			v.push_back(first->second);
		}
		return v;
	}

	const_exam_it School::hold_exam(std::istream& in, const numb_type& Exam_Numb, const name_type& Exam_Name, const weight_type& _Weight)
	{
		if (_Check_Exam_Numb(Exam_Numb) && _Check_Istream(in))
		{
			std::string m_str;
			const_stu_it_mark_vec mVec;

			while (!in.eof())
			{
				if (!(in >> m_str)) break;
				auto&& m_num = stoull(m_str);
				auto&& _pos = _find_stu(m_num);
				in >> m_str;
				if (_pos == _Stu_List.end())
				{
					_pos = _add_stu_unchecked(m_num, m_str);
				}
				in >> m_str;
				auto&& m_mark = stod(m_str);
				mVec.emplace_back(_pos, m_mark);
			}
			auto&& nExamIt = _add_exam_unchecked(mVec, Exam_Numb, Exam_Name, _Weight);

			for (auto& v : mVec)
			{
				const_cast<Student&>(*v.first)._add_exam(nExamIt, v.second);// 抛出常量属性以减少拷贝次数来提高效率
			}
			return nExamIt;
		}
		else return _Exam_List.cend();
	}

	const_stu_it_vec School::Vec_const_stu_it(SortedType _Type) const noexcept
	{
		const_stu_it_vec mVec;
		for (auto&& _Ita = _Stu_List.begin(); _Ita != _Stu_List.end(); ++_Ita)
		{
			mVec.push_back(_Ita);
		}

		switch (_Type)
		{
		case xhx::School::Sorted_by_Numb_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::School::Sorted_by_Numb_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::School::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::School::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::School::Sorted_by_Mark_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Left->Weighted_Mark() < _Right->Weighted_Mark();
				}
			);
			break;
		case xhx::School::Sorted_by_Mark_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Right->Weighted_Mark() < _Left->Weighted_Mark();
				}
			);
			break;
		case xhx::School::Sorted_by_Weight_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Left->Credit() < _Right->Credit();
				}
			);
			break;
		case xhx::School::Sorted_by_Weight_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Right->Credit() < _Left->Credit();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}

	const_exam_it_vec School::Vec_const_exam_it(SortedType _Type) const noexcept
	{
		const_exam_it_vec mVec;
		for (auto&& _Ita = _Exam_List.begin(); _Ita != _Exam_List.end(); ++_Ita)
		{
			mVec.push_back(_Ita);
		}

		switch (_Type)
		{
		case xhx::School::Sorted_by_Numb_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::School::Sorted_by_Numb_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::School::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::School::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::School::Sorted_by_Mark_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Left->_Anal.mean < _Right->_Anal.mean;
				}
			);
			break;
		case xhx::School::Sorted_by_Mark_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Right->_Anal.mean < _Left->_Anal.mean;
				}
			);
			break;
		case xhx::School::Sorted_by_Weight_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Left->Weight() < _Right->Weight();
				}
			);
			break;
		case xhx::School::Sorted_by_Weight_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_exam_it& _Left, const const_exam_it& _Right)->bool
				{
					return _Right->Weight() < _Left->Weight();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}

	const_stu_it_rank_vec School::Vec_const_stu_it_rank() const noexcept
	{
		const_stu_it_rank_vec mVec;
		for (auto&& _Ita = _Stu_List.begin(); _Ita != _Stu_List.end(); ++_Ita)
		{
			mVec.emplace_back(_Ita, rank_type(1));
		}
		std::sort
		(
			mVec.begin(), mVec.end(),
			[](const const_stu_it_rank& _Left, const const_stu_it_rank& _Right)->bool
			{
				return _Left.first->Weighted_Mark() > _Right.first->Weighted_Mark();
			}
		);
		for (auto&& _It1 = mVec.begin(), && _It2 = _It1 + 1; _It2 < mVec.end(); ++_It1, ++_It2)
		{
			_It2->second = _It1->first->Weighted_Mark() == _It2->first->Weighted_Mark() ? _It1->second : _It2 - mVec.begin() + 1;
		}
		return mVec;
	}

	const_stu_it_rank_vec School::Vec_const_stu_it_rank(SortedType _Type) const noexcept
	{
		auto&& mVec = Vec_const_stu_it_rank();

		switch (_Type)
		{
		case xhx::School::Sorted_by_Numb_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::School::Sorted_by_Numb_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::School::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::School::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::School::Sorted_by_Mark_Non_Descending:
			std::reverse(mVec.begin(), mVec.end());
			break;
		case xhx::School::Sorted_by_Mark_Non_Ascending:
			// nothing to do
			break;
		case xhx::School::Sorted_by_Weight_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it_rank& _Left, const const_stu_it_rank& _Right)->bool
				{
					return _Left.first->Credit() < _Right.first->Credit();
				}
			);
			break;
		case xhx::School::Sorted_by_Weight_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it_rank& _Left, const const_stu_it_rank& _Right)->bool
				{
					return _Right.first->Credit() < _Left.first->Credit();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}

	void School::output_stu(std::ostream& out, SortedType _Type) const
	{
		auto&& mVec = Vec_const_stu_it_rank(_Type);

		out << "    学号   姓名   学分  加权成绩  排名\n";
		auto&& _Begin = mVec.begin();
		for (size_t _cnt = 1; _Begin != mVec.end(); ++_Begin, ++_cnt)
		{
			_output_stu(out, _Begin->first, _Begin->second, _cnt);
		}
	}

	void School::output_exam(std::ostream& out, SortedType _Type) const
	{
		auto&& mVec = Vec_const_exam_it(_Type);
		out << "    考试编号 考试名称 考试人数 课程学分  平均分\n";
		auto&& _Begin = mVec.begin();
		for (size_t _cnt = 1; _Begin != mVec.end(); ++_Begin, ++_cnt)
		{
			_output_exam(out, *_Begin, _cnt);
		}
	}

	stu_iterator School::_find_stu(const numb_type& Stu_Numb) noexcept
	{
		auto&& _pos = _Map_Stu_numb.find(Stu_Numb);
		return _pos == _Map_Stu_numb.end() ? _Stu_List.end() : _pos->second;
	}

	stu_it_vector School::_find_stu(const name_type& Stu_Name) noexcept
	{
		stu_it_vector v;
		for (auto&& first = _Map_Stu_name_numb.lower_bound(Stu_Name); first != _Map_Stu_name_numb.end() && first->second->Name() == Stu_Name; ++first)
		{                                      // 此处发生了隐式类型转换:string -> Object
			v.push_back(first->second);
		}
		return v;
	}

	exam_iterator School::_find_exam(const numb_type& Exam_Numb) noexcept
	{
		auto&& _pos = _Map_Exam_numb.find(Exam_Numb);
		return _pos == _Map_Exam_numb.end() ? _Exam_List.end() : _pos->second;
	}

	exam_it_vector School::_find_exam(const name_type& Exam_Name) noexcept
	{
		exam_it_vector v;
		for (auto&& first = _Map_Exam_name_numb.lower_bound(Exam_Name); first != _Map_Exam_name_numb.end() && first->second->Name() == Exam_Name; ++first)
		{                                      // 此处发生了隐式类型转换:string -> Object
			v.push_back(first->second);
		}
		return v;
	}

	stu_iterator School::_add_stu_unchecked(const numb_type& Stu_Numb, const name_type& Stu_Name) noexcept
	{
		_Stu_List.emplace_back(Stu_Numb, Stu_Name);
		auto& _It = _Map_Stu_numb.emplace(Stu_Numb, --_Stu_List.end()).first->second;
		_Map_Stu_name_numb.emplace(name_numb{ Stu_Name, Stu_Numb }, _It);
		return _It;
	}

	exam_iterator School::_add_exam_unchecked(const const_stu_it_mark_vec& _Vec, const numb_type& Exam_Numb, const name_type& Exam_Name, const weight_type& _Weight) noexcept
	{
		_Exam_List.emplace_back(*this, _Vec, Exam_Numb, Exam_Name, _Weight);
		return --_Exam_List.end();
	}


	inline bool School::_Check_Stu_Numb(const numb_type& Stu_Numb) const noexcept
	{
		return _Map_Stu_numb.find(Stu_Numb) == _Map_Stu_numb.cend();
	}

	inline bool School::_Check_Exam_Numb(const numb_type& Exam_Numb) const noexcept
	{
		return _Map_Exam_numb.find(Exam_Numb) == _Map_Exam_numb.cend();
	}

	inline bool School::_Check_Istream(std::istream& _src)
	{
		std::string m_str;
		size_t _cnt;
		_src.clear();
		_src.seekg(0, std::ios::beg);
		for (_cnt = 0; _src.good(); ++_cnt)
		{
			if (_src >> m_str)
			{
				if (m_str.length() < 19)
				{
					for (const auto& i : m_str)
					{
						if (i < '0' || '9' < i)
						{
							_src.seekg(0, std::ios::beg);
							return false;
						}
					}
				}
				else
				{
					_cnt = 0;
					break;
				}
			}
			if (_src >> m_str >> m_str)
			{
				bool _dot = true;
				for (const auto& i : m_str)
				{
					if (i == '.' && _dot)
					{
						_dot = false;
					}
					else if (i < '0' || '9' < i)
					{
						_src.seekg(0, std::ios::beg);
						return false;
					}
				}
			}
		}
		_src.clear();
		_src.seekg(0, std::ios::beg);
		return _cnt > 0;
	}

	void School::_output_stu(std::ostream& out, const const_stu_it& _Stu_CIt, const rank_type& _Rank, const size_t& _Order)
	{
		const auto& _Stu = *_Stu_CIt;
		if (_Order)
		{
			out << _Order << _Blanks;
		}
		out << _Stu.Numb() << _Blanks << _Stu.Name() << _Blanks << _Stu.Credit()
			<< _Blanks << _Stu.Weighted_Mark() << _Blanks << _Rank << '\n';
	}

	void School::_output_exam(std::ostream& out, const const_exam_it& _Exam_CIt, const size_t& _Order)
	{
		const auto& _Exam = *_Exam_CIt;
		if (_Order)
		{
			out << _Order << _Blanks;
		}
		out << _Exam.Numb() << _Blanks << _Exam.Name() << _Blanks << _Exam.Count()
			<< _Blanks << _Exam.Weight() << _Blanks << _Exam._Anal.mean << '\n';
	}

	Object::Object(const numb_type& _Numb, const name_type& _Name)
		:_Numb(_Numb), _Name(_Name) {}

	Object::Object(const name_type& _Name, const numb_type& _Numb)
		: Object(_Numb, _Name) {}

	const inline name_type& Object::Name() const noexcept
	{
		return _Name;
	}

	const inline numb_type& Object::Numb() const noexcept
	{
		return _Numb;
	}

	Exam::Exam(const School& _Sch, const const_stu_it_mark_vec& _Vec, const numb_type& _Numb, const name_type& _Name, weight_type _Weight)
		: Object(_Numb, _Name), _Sch(_Sch), _Weight(_Weight), _Count(_Vec.size()), _Stu_It_Mark_Vec(_Vec), _Anal(*this)
	{
	}

	const size_t& Exam::Count() const noexcept
	{
		return _Count;
	}

	const weight_type& Exam::Weight() const noexcept
	{
		return _Weight;
	}

	const School& Exam::in_School() const noexcept
	{
		return _Sch;
	}

	const_stu_it Exam::find_stu(const numb_type& Stu_Numb) const noexcept
	{
		auto _Where = lower_bound
		(
			_Stu_It_Mark_Vec.begin(), _Stu_It_Mark_Vec.end(), Stu_Numb,
			[](const const_stu_it_mark& _Left, const numb_type& _Right)->bool
			{
				return _Left.first->Numb() < _Right;
			}
		);
		return _Where != _Stu_It_Mark_Vec.end() ? _Where->first : _Stu_It_Mark_Vec.rbegin()->first;
	}

	const_stu_it_vec Exam::find_stu(const name_type& Stu_Name) const noexcept
	{
		const_stu_it_vec v;
		for (auto&& first = lower_bound(v.begin(), v.end(), Stu_Name, less_Name()); first != v.end() && (*first)->Name() == Stu_Name; ++first)
		{
			v.push_back(*first);
		}
		return v;
	}

	mark_type Exam::get_mark(const numb_type& Stu_Numb) const noexcept
	{
		auto&& _Where = lower_bound
		(
			_Stu_It_Mark_Vec.begin(), _Stu_It_Mark_Vec.end(), Stu_Numb,
			[](const const_stu_it_mark& _Left, const numb_type& _Right)->bool
			{
				return _Left.first->Numb() < _Right;
			}
		);
		return _Where != _Stu_It_Mark_Vec.end() ? _Where->second : Mark_Invalid;
	}

	mark_rank Exam::get_mark_rank(const numb_type& Stu_Numb) const noexcept
	{
		auto&& _Mark = get_mark(Stu_Numb);
		return mark_rank(_Mark, get_rank2(_Mark));
	}

	rank_type Exam::get_rank(const numb_type& Stu_Numb) const noexcept
	{
		return get_rank2(get_mark(Stu_Numb));
	}

	rank_type Exam::get_rank2(const mark_type& _Mark) const noexcept
	{
		auto&& _Where = lower_bound
		(
			_Mark_Table.begin(), _Mark_Table.end(), _Mark,
			[](const mark_rank& _Left, const mark_type& _Right)->bool
			{
				return _Left.first > _Right;
			}
		);
		return _Where != _Mark_Table.end() ? _Where->second : rank_type(0);
	}

	const_stu_it_mark_vec Exam::Vec_const_stu_it_mark(SortedType _Type) const noexcept
	{
		const_stu_it_mark_vec mVec(_Stu_It_Mark_Vec);

		switch (_Type)
		{
		case xhx::Exam::Sorted_by_Numb_Non_Descending:
			// nothing to do
			break;
		case xhx::Exam::Sorted_by_Numb_Non_Ascending:
			std::reverse(mVec.begin(), mVec.end());
			break;
		case xhx::Exam::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::Exam::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::Exam::Sorted_by_Mark_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it_mark& _Left, const const_stu_it_mark& _Right)->bool
				{
					return _Left.first->Weighted_Mark() < _Right.first->Weighted_Mark();
				}
			);
			break;
		case xhx::Exam::Sorted_by_Mark_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it_mark& _Left, const const_stu_it_mark& _Right)->bool
				{
					return _Right.first->Weighted_Mark() < _Left.first->Weighted_Mark();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}

	const_stu_it_vec Exam::Vec_const_stu_it(SortedType _Type) const noexcept
	{
		const_stu_it_vec mVec;
		for (const auto& v : _Stu_It_Mark_Vec)
		{
			mVec.push_back(v.first);
		}

		switch (_Type)
		{
		case xhx::Exam::Sorted_by_Numb_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Numb());
			break;
		case xhx::Exam::Sorted_by_Numb_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Numb());
			break;
		case xhx::Exam::Sorted_by_Name_Non_Descending:
			std::sort(mVec.begin(), mVec.end(), less_Name());
			break;
		case xhx::Exam::Sorted_by_Name_Non_Ascending:
			std::sort(mVec.rbegin(), mVec.rend(), less_Name());
			break;
		case xhx::Exam::Sorted_by_Mark_Non_Descending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Left->Weighted_Mark() < _Right->Weighted_Mark();
				}
			);
			break;
		case xhx::Exam::Sorted_by_Mark_Non_Ascending:
			std::sort
			(
				mVec.begin(), mVec.end(),
				[](const const_stu_it& _Left, const const_stu_it& _Right)->bool
				{
					return _Right->Weighted_Mark() < _Left->Weighted_Mark();
				}
			);
			break;
		default:
			break;
		}
		return mVec;
	}


	void Exam::output(std::ostream& out, SortedType _Type) const
	{
		const_stu_it_mark_vec&& mVec = Vec_const_stu_it_mark(_Type);

		out << *this << "    学号   姓名   分数   排名\n";
		auto&& _Begin = mVec.begin();
		for (size_t _cnt = 1; _Begin != mVec.end(); ++_Begin, ++_cnt)
		{
			const auto& v = *_Begin;
			_output(out, v.first, v.second, get_rank2(v.second), _cnt);
		}
	}

	void Exam::_output(std::ostream& out, const numb_type& _Stu_Numb, const name_type& _Stu_Name, const mark_type& _Mark, const rank_type& _Rank, const size_t& _Order)
	{
		if (_Order)
		{
			out << _Order << _Blanks;
		}
		out << _Stu_Numb << _Blanks << _Stu_Name << _Blanks << _Mark << _Blanks << _Rank << '\n';
	}

	void Exam::_output(std::ostream& out, const const_stu_it& _Stu_CIt, const mark_type& _Mark, const rank_type& _Rank, const size_t& _Order)
	{
		return _output(out, _Stu_CIt->Numb(), _Stu_CIt->Name(), _Mark, _Rank, _Order);
	}

	Exam::_Analyzer::_Analyzer(Exam& _Exam)
	{
		auto& _Stu_It_Mark_Vec = _Exam._Stu_It_Mark_Vec;
		auto& _Stu_It_Vec_Name = _Exam._Stu_It_Vec_Name;
		auto& _Mark_Table = _Exam._Mark_Table;
		const auto& _Cnt = _Exam._Count;

		mean = 0;
		for (const auto& v : _Stu_It_Mark_Vec)
		{
			_Stu_It_Vec_Name.push_back(v.first);
			_Mark_Table.emplace_back(v.second, rank_type(1));
			mean += v.second;
		}
		std::sort(_Stu_It_Mark_Vec.begin(), _Stu_It_Mark_Vec.end(), less_Numb());
		std::sort(_Stu_It_Vec_Name.begin(), _Stu_It_Vec_Name.end(), less_Name());
		std::sort(_Mark_Table.rbegin(), _Mark_Table.rend());
		mean /= _Cnt;
		highest = _Mark_Table.begin()->first;
		lowest = (_Mark_Table.end() - 1)->first;

		for (auto&& _It1 = _Mark_Table.begin(), && _It2 = _It1 + 1; _It2 < _Mark_Table.end(); ++_It1, ++_It2)
		{
			_It2->second = _It1->first == _It2->first ? _It1->second : _It2 - _Mark_Table.begin() + 1;
		}

		standard_deviation = 0;
		for (auto& v : _Mark_Table)
		{
			const auto&& m = v.first - mean;
			standard_deviation += m * m;
		}
		standard_deviation = std::sqrt(standard_deviation / _Cnt);
		std::unique(_Mark_Table.begin(), _Mark_Table.end());

		_Stu_It_Mark_Vec.shrink_to_fit();
		_Stu_It_Vec_Name.shrink_to_fit();
		_Mark_Table.shrink_to_fit();
	}

	std::ostream& operator<<(std::ostream& out, const Student& stu)
	{
		return out << "  学生编号：" << stu._Numb << "  学生姓名：" << stu._Name << "  参加考试门数：" << stu._Exams.size()
			<< "  总学分：" << stu._Credit << "  加权成绩：" << stu.Weighted_Mark() << "\n\n";
	}

	std::ostream& operator<<(std::ostream& out, const Exam& _Exam)
	{
		return out << "  考试编号：" << _Exam._Numb << "  考试人数：" << _Exam._Count
			<< "  考试权重值：" << _Exam._Weight << "\n考试情况分析：\n" << _Exam._Anal << "\n\n";
	}

	std::ostream& operator<<(std::ostream& out, const Exam::_Analyzer& anal)
	{
		return out << "\n最高分：" << anal.highest << "  最低分：" << anal.lowest
			<< "  平均分：" << anal.mean << "  标准差：" << anal.standard_deviation << '\n';
	}


	template<typename ItL, typename ItR>
	inline bool less_Numb::operator()(const ItL& _Left, const ItR& _Right) const noexcept
	{
		return _Left->Numb() < _Right->Numb();
	}

	template<typename First1, typename Second1, typename First2, typename Second2>
	inline bool less_Numb::operator()(const std::pair<First1, Second1>& _Left, const std::pair<First2, Second2>& _Right) const noexcept
	{
		return _Left.first->Numb() < _Right.first->Numb();
	}

	template<typename ItR>
	inline bool less_Numb::operator()(const numb_type& _Left, const ItR& _Right) const noexcept
	{
		return _Left < _Right->Numb();
	}

	template<typename ItL>
	inline bool less_Numb::operator()(const ItL& _Left, const numb_type& _Right) const noexcept
	{
		return _Left->Numb() < _Right;
	}

	inline bool less_Numb::operator()(const Object& _Left, const Object& _Right) const noexcept
	{
		return _Left.Numb() < _Right.Numb();
	}

	inline bool less_Numb::operator()(const numb_type& _Left, const numb_type& _Right) const noexcept
	{
		return _Left < _Right;
	}

	inline bool less_Name::operator()(const Object& _Left, const Object& _Right) const noexcept
	{
		const auto&& _cmpres = _Left.Name().compare(_Right.Name());
		return _cmpres < 0 || _cmpres == 0 && _Left.Numb() < _Right.Numb();
	}

	inline bool less_Name::operator()(const name_type& _Left, const name_type& _Right) const noexcept
	{
		return _Left < _Right;
	}

	template<typename ItL, typename ItR>
	inline bool less_Name::operator()(const ItL& _Left, const ItR& _Right) const noexcept
	{
		return operator()((Object&)* _Left, (Object&)* _Right);
	}

	template<typename First1, typename Second1, typename First2, typename Second2>
	inline bool less_Name::operator()(const std::pair<First1, Second1>& _Left, const std::pair<First2, Second2>& _Right) const noexcept
	{
		return operator()((Object&)* _Left.first, (Object&)* _Right.first);
	}

	template<typename ItR>
	inline bool less_Name::operator()(const name_type& _Left, const ItR& _Right) const noexcept
	{
		return _Left < _Right->Name();
	}

	template<typename ItL>
	inline bool less_Name::operator()(const ItL& _Left, const name_type& _Right) const noexcept
	{
		return _Left->Name() < _Right;
	}


	template<typename RanIt>
	void Exam::_output_range(std::ostream& out, RanIt _Begin, RanIt _End) const
	{
		for (size_t _cnt = 1; _Begin != _End; ++_Begin, ++_cnt)
		{
			const auto& _Cur = *_Begin;
			_output(out, _Cur.first, _Cur.second, get_rank2(_Cur.second), _cnt);
		}
	}

}








// 以下是成绩管理系统的逻辑实现, 主要以调用 xhx 中的函数完成


using namespace std;
using namespace xhx;



const char* const State[] =
{
	"添加一次考试",
	"查找 并 查看 考试情况",
	"查找 并 查看 学生考试情况",
	"输出全部 考试信息 及 考试结果",
	"输出全部 学生信息 及 成绩单",
};

void in_add_exam();
void in_get_exam(bool _Type);
void in_get_stu(bool _Type);
void in_output_exam();
void in_output_stu();
bool return_or_stay(int st);
string in_get_path();

School Sch(0, "天津大学");

int main()
{

	for (int Oper = -1; Oper;)
	{
		cout <<
			"\n\n 1. 添加一次考试\n\n"
			" 2. 按 考试编号 查找 并 查看 考试情况\n"
			" 3. 按 考试名称 查找 并 查看 考试情况\n\n"
			" 4. 按 学号 查找 并 查看 学生考试情况\n"
			" 5. 按 姓名 查找 并 查看 学生考试情况\n\n"
			" 6. 输出 全部考试 的 考试情况\n"
			" 7. 输出 全部学生 的 考试情况\n\n"
			" 0. 输入 0 退出系统\n\n\n请输入：";
		cin >> Oper;

		switch (Oper)
		{
		case 1:
			in_add_exam();
			break;
		case 2:
			in_get_exam(false);
			break;
		case 3:
			in_get_exam(true);
			break;
		case 4:
			in_get_stu(false);
			break;
		case 5:
			in_get_stu(true);
			break;
		case 6:
			in_output_exam();
			break;
		case 7:
			in_output_stu();
			break;
		case 0:
			cout << "\n\n\n\n\n   欢迎使用本系统    将于3秒后退出\n\n\n\n\n";
			auto c1 = clock();
			while (clock() - c1 < CLOCKS_PER_SEC * 3);
			break;
		}
	}

	return 0;
}

void in_add_exam()
{
	for (bool F = true; F || return_or_stay(0); F = false)
	{
		numb_type numb;
		string name;
		weight_type weight;

		cout << "\n\n  请输入考试编号：";
		cin >> numb;
		auto _EIt = Sch.find_exam(numb);
		if (_EIt == Sch.Exam_List().cend())
		{
			cout << "\n  请输入考试名称：";
			cin >> name;
			cout << "\n  请输入考试权重：";
			cin >> weight;
			fstream m_fstr(in_get_path(), ios::in);
			string sss;

			if (m_fstr.good())
			{
				_EIt = Sch.hold_exam(m_fstr, numb, name, weight);
				if (_EIt != Sch.Exam_List().cend())
				{
					cout << "\n  添加考试成功！\n" << *_EIt << '\n';
					break;
				}
				else
				{
					cout << "\n\n  添加失败：文件有错\n";
				}
			}
			else
			{
				cout << "\n\n  添加失败：无法打开文件\n";
			}
		}
		else
		{
			cout << "\n\n  添加失败：发现重复编号的考试\n" << *_EIt << '\n';
		}
	}
}

void in_get_exam(bool _Type)
{
	const_exam_it _EIt;

	if (_Type)
	{
		string name;
		cout << "\n\n  请输入要查询的考试名称：";
		cin >> name;

		for (auto v = Sch.find_exam(name); !v.size(); v = Sch.find_exam(name))
		{
			if (!v.size())
			{
				cout << "\n\n  未查询到名称为 " << name
					<< " 的考试\n\n  请重新输入要查询的考试名称：";
				cin >> name;
			}
			else if (v.size() == 1)
			{
				cout << "\n\n  查找成功!\n" << *_EIt;
				_EIt = *v.begin();
				break;
			}
			else
			{
				cout << "\n\n  共查找到 " << v.size() << " 个名称为 " << name << " 的考试：";
				for (auto& i : v)
				{
					cout << *i << '\n';
				}
				_Type = false;
				break;
			}
		}
	}
	if (!_Type)
	{
		numb_type numb;
		cout << "\n\n  请输入要查询的考试编号：";
		cin >> numb;
		for (_EIt = Sch.find_exam(numb);; _EIt = Sch.find_exam(numb))
		{
			if (_EIt != Sch.Exam_List().cend())
			{
				cout << "\n\n  查找成功!\n" << *_EIt << '\n';
				break;
			}
			else
			{
				cout << "\n\n  未查询到编号为 " << numb << " 的考试\n\n  请重新输入考试编号：";
				cin >> numb;
			}
		}
	}
	auto& Exm = *_EIt;

	for (int Oper = -1; Oper;)
	{
		cout << "    请输入操作：\n"
			" 1. 输入学号  查找 并 输出 单个学生考试成绩和排名\n"
			" 2. 输入姓名  查找 并 输出 单个学生考试成绩和排名\n"
			" 3. 输出考试成绩单\n"
			" 4. 输出考试成绩分析\n"
			" 0. 返回上一级\n\n  请输入：";
		cin >> Oper;

		switch (Oper)
		{
		case 1:
		{
			numb_type m;
			cout << "\n\n  请输入要查询的学号：";
			cin >> m;

			auto tar = *Exm.find_stu(m);
			if (tar.Numb() == m)
			{
				auto res = Exm.get_mark_rank(tar.Numb());
				cout << "\n  查找成功：\n\n   学号：" << tar.Numb() << "  姓名：" << tar.Name()
					<< "  分数：" << res.first << "  排名：" << res.second << "\n\n";
			}
			else
			{
				cout << "\n  本场考试中未找到学号为 " << m << " 的学生\n\n";
			}
		}
		break;
		case 2:
		{
			name_type m;
			cout << "\n\n  请输入要查询的姓名：";
			cin >> m;

			auto&& tv = Exm.find_stu(m);
			cout << "\n\n  本场考试中查找到 " << tv.size() << " 个姓名为 " << m << " 的学生\n";
			for (const auto& i : tv)
			{
				const auto& tar = *i;
				auto res = Exm.get_mark_rank(tar.Numb());
				cout << "\n   学号：" << tar.Numb() << "  姓名：" << tar.Name()
					<< "  分数：" << res.first << "  排名：" << res.second << "\n";
			}
		}
		break;
		case 3:
		{
			cout << "请输入排序方式：\n\n"
				" 1.按学号非降序排序\n"
				" 2.按学号非升序排序\n"
				" 3.按姓名字典序非降序排序\n"
				" 4.按姓名字典序非升序排序\n"
				" 5.按分数非降序排序(分数相同按学号升序排序)\n"
				" 6.按分数非升序排序(分数相同按学号降序排序)\n\n"
				"   请输入：";
			int styp;
			cin >> styp;
			styp += 20;
			cout << "\n\n  请输入要输出的文件位置：(输入一个0则输出至屏幕)\n";
			auto pth = in_get_path();

			if (pth[0] == '0')
			{
				Exm.output(cout, Exam::SortedType(styp));
			}
			else
			{
				fstream m_fstr(pth, ios::out | ios::app);
				if (m_fstr)
				{
					m_fstr << '\n' << Exm << '\n';
					Exm.output(m_fstr, Exam::SortedType(styp));
				}
				else
				{
					cout << "\n  无法打开路径！\n\n";
				}
			}
		}
		break;
		case 4:
			cout << '\n' << Exm << '\n';
			break;
		case 0:
			break;
		default:
			break;
		}
	}
}

void in_get_stu(bool _Type)
{
	const_stu_it _SIt;
	if (_Type)
	{
		string name;
		cout << "\n\n  请输入要查询的学生姓名：";
		cin >> name;

		for (auto v = Sch.find_stu(name); !v.size(); v = Sch.find_stu(name))
		{
			if (!v.size())
			{
				cout << "\n\n  未查询到姓名为 " << name
					<< " 的学生\n\n  请重新输入要查询的学生姓名：";
				cin >> name;
			}
			else if (v.size() == 1)
			{
				cout << "\n\n  查找成功!\n" << *_SIt;
				_SIt = *v.begin();
				break;
			}
			else
			{
				cout << "\n\n  共查找到 " << v.size() << " 个名称为 " << name << " 的学生：";
				for (auto& i : v)
				{
					cout << *i << '\n';
				}
				_Type = false;
				break;
			}
		}
	}
	if (!_Type)
	{
		numb_type numb;
		cout << "\n\n  请输入要查询的学号：";
		cin >> numb;
		for (_SIt = Sch.find_stu(numb);; _SIt = Sch.find_stu(numb))
		{
			if (_SIt != Sch.Stu_List().cend())
			{
				cout << "\n\n  查找成功!\n" << *_SIt << '\n';
				break;
			}
			else
			{
				cout << "\n\n  未查询到编号为 " << numb << " 的考试\n\n  请重新输入考试编号：";
				cin >> numb;
			}
		}
	}
	auto& Stu = *_SIt;

	for (int Oper = -1; Oper;)
	{
		cout << "    请输入操作：\n"
			" 1. 输出考试成绩单\n"
			" 0. 返回上一级\n\n  请输入：";
		cin >> Oper;

		switch (Oper)
		{
		case 1:
		{			cout << "请输入排序方式：\n\n"
			" 1.按时间顺序排序\n"
			" 2.按时间逆序排序\n"
			" 3.按考试编号非降序排序\n"
			" 4.按考试编号非升序排序\n"
			" 5.按考试名称字典序非降序排序\n"
			" 6.按考试名称字典序非升序排序\n"
			" 7.按分数非降序排序\n"
			" 8.按分数非升序排序\n"
			" 9.按学分非降序排序\n"
			" 10.按学分非升序排序\n\n"
			"   请输入：";
		int styp;
		cin >> styp;
		styp += 10;
		Stu.output(cout, Student::SortedType(styp));
		}
		default:
			break;
		}
	}
}

void in_output_exam()
{
	for (;;)
	{
		cout << "请输入排序方式：\n\n"
			" 1.按编号非降序排序\n"
			" 2.按编号非升序排序\n"
			" 3.按名称字典序非降序排序\n"
			" 4.按名称字典序非升序排序\n"
			" 5.按分数非降序排序(分数相同按编号升序排序)\n"
			" 6.按分数非升序排序(分数相同按编号降序排序)\n"
			" 7.按学分 / 权重非降序排序\n"
			" 8.按学分 / 权重非升序排序\n\n"
			"   请输入：";
		int styp;
		cin >> styp;
		styp += 30;

		cout << "\n\n  请输入要输出的文件位置：(输入一个0则输出至屏幕)\n";
		auto pth = in_get_path();

		if (pth[0] == '0')
		{
			Sch.output_exam(cout, School::SortedType(styp));
			break;
		}
		else
		{
			fstream m_fstr(pth, ios::out | ios::app);
			if (m_fstr)
			{
				Sch.output_exam(m_fstr, School::SortedType(styp));
				break;
			}
			else
			{
				cout << "\n  无法打开路径！\n\n";
			}
		}
	}
}

void in_output_stu()
{
	for (;;)
	{
		cout << "请输入排序方式：\n\n"
			" 1.按编号非降序排序\n"
			" 2.按编号非升序排序\n"
			" 3.按名称字典序非降序排序\n"
			" 4.按名称字典序非升序排序\n"
			" 5.按分数非降序排序(分数相同按编号升序排序)\n"
			" 6.按分数非升序排序(分数相同按编号降序排序)\n"
			" 7.按学分 / 权重非降序排序\n"
			" 8.按学分 / 权重非升序排序\n\n"
			"   请输入：";
		int styp;
		cin >> styp;
		styp += 30;
		cout << "\n\n  请输入要输出的文件位置：(输入一个0则输出至屏幕)\n";
		auto pth = in_get_path();

		if (pth[0] == '0')
		{
			Sch.output_stu(cout, School::SortedType(styp));
			break;
		}
		else
		{
			fstream m_fstr(pth, ios::out | ios::app);
			if (m_fstr)
			{
				Sch.output_stu(m_fstr, School::SortedType(styp));
				break;
			}
			else
			{
				cout << "\n  无法打开路径！\n\n";
			}
		}
	}
}

bool return_or_stay(int st)
{
	cout << "\n\n  是否停留在 " << State[st] << " ? (输入 1停留   0返回主菜单)";
	string m;
	for (;;)
	{
		cin >> m;
		if (m[0] == '1')
		{
			return true;
		}
		else if (m[0] == '0')
		{
			return false;
		}
	}
}

string in_get_path()
{
	string res;
	cout << "\n\n  请输入完整文件路径：";
	cin >> res;
	cout << "\n\n";
	return res;
}
