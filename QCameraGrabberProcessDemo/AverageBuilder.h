#ifndef AVERAGE_BUILDER_H
#define AVERAGE_BUILDER_H


template <typename t = double, size_t ExpectedCount = 10>
class AverageBuilder
{
	static_assert(std::is_arithmetic<t>::value, "AverageBuilder template argument t must be arithmetic.");
	static_assert(ExpectedCount > 0, "AverageBuilder template argument ExpectedCount must be > 0.");

public:
	void add(t value) { 
		if (mCurrentCount < ExpectedCount) {
			mAverageValue = (mAverageValue * mCurrentCount + value) / (mCurrentCount + 1);
			++mCurrentCount;
		} else {
			mAverageValue = (mAverageValue * (ExpectedCount - 1) + value) / ExpectedCount;
		}
	}

	t average() const { return mAverageValue; }
	size_t count() const { return mCurrentCount; }

private:
	size_t mCurrentCount{ 0 };
	t mAverageValue{ static_cast<t>(0) };
};


#endif // AVERAGE_BUILDER_H

