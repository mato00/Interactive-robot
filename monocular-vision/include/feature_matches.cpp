#include "feature_matches.h"

int feature_matches::GetInlierMask(vector<bool> &vbInliers, bool WithScale, bool WithRotation) {

	int max_inlier = 0;

	if (!WithScale && !WithRotation)
	{
		max_inlier = run(1, 0);
		vbInliers = mvbInlierMask;
		return max_inlier;
	}


	if (WithRotation && WithScale)
	{
		for (int Scale = 0; Scale < 5; Scale++)
		{
			for (int RotationType = 1; RotationType <= 8; RotationType++)
			{
				int num_inlier = run(RotationType, Scale);

				if (num_inlier > max_inlier)
				{
					vbInliers = mvbInlierMask;
					max_inlier = num_inlier;
				}
			}
		}
		return max_inlier;
	}

	if (WithRotation && !WithScale)
	{

		for (int RotationType = 1; RotationType <= 8; RotationType++)
		{
			int num_inlier = run(RotationType, 0);

			if (num_inlier > max_inlier)
			{
				vbInliers = mvbInlierMask;
				max_inlier = num_inlier;
			}
		}
		return max_inlier;
	}

	if (!WithRotation && WithScale)
	{
		for (int Scale = 0; Scale < 5; Scale++)
		{
			int num_inlier = run(1, Scale);

			if (num_inlier > max_inlier)
			{
				vbInliers = mvbInlierMask;
				max_inlier = num_inlier;
			}

		}
		return max_inlier;
	}

	return max_inlier;
}

void feature_matches::AssignMatchPairs(int GridType) {

	for (size_t i = 0; i < mNumberMatches; i++)
	{
		Point2f &lp = mvP1[mvMatches[i].first];
		Point2f &rp = mvP2[mvMatches[i].second];

		const int lgidx = mvMatchPairs[i].first = GetGridIndexLeft(lp, GridType);
		const int rgidx = mvMatchPairs[i].second = GetGridIndexRight(rp);

		if(lgidx < 0 || rgidx < 0)	continue;

		mMotionStatistics[lgidx][rgidx] ++;
		mNumberPointsInPerCellLeft[lgidx]++;
	}
}

void feature_matches::VerifyCellPairs(int RotationType) {

	const int *CurrentRP = mRotationPatterns[RotationType - 1];

	for (int i = 0; i < mGridNumberLeft; i++)
	{
		if (mMotionStatistics[i].empty())
		{
			mCellPairs[i] = -1;
			continue;
		}

		int max_number = 0;
		for (map<int, int>::iterator it = mMotionStatistics[i].begin(); it != mMotionStatistics[i].end(); ++it)
		{
			if (it->second > max_number)
			{
				mCellPairs[i] = it->first;
				max_number = it->second;
			}
		}



		int idx_grid_rt = mCellPairs[i];

		vector<int> NB9_lt = GetNB9(i, mGridSizeLeft);
		vector<int> NB9_rt = GetNB9(idx_grid_rt, mGridSizeRight);

		int score = 0;
		double thresh = 0;
		int numpair = 0;

		for (size_t j = 0; j < 9; j++)
		{
			int ll = NB9_lt[j];
			int rr = NB9_rt[CurrentRP[j] - 1];
			if (ll == -1 || rr == -1)	continue;

			score += mMotionStatistics[ll][rr];
			thresh += mNumberPointsInPerCellLeft[ll];
			numpair++;
		}

		thresh = THRESH_FACTOR * sqrt(thresh / numpair);

		if (score < thresh)
			mCellPairs[i] = -2;
	}

}

int feature_matches::run(int RotationType, int Scale) {

	mvbInlierMask.assign(mNumberMatches, false);
	for (int GridType = 1; GridType <= 4; GridType++)
	{
		// Set Scale
		mGridSizeRight.width  = mGridSizeLeft.width  * mScaleRatios[Scale];
		mGridSizeRight.height = mGridSizeLeft.height * mScaleRatios[Scale];

		// initialize
		mMotionStatistics.assign(mGridNumberLeft, map<int, int>());
		mCellPairs.assign(mGridNumberLeft, -1);
		mNumberPointsInPerCellLeft.assign(mGridNumberLeft, 0);
		mvMatchPairs.assign(mNumberMatches, pair<int, int>(0, 0));

		AssignMatchPairs(GridType);
		VerifyCellPairs(RotationType);

		// Mark inliers
		for (size_t i = 0; i < mNumberMatches; i++)
		{
			if (mCellPairs[mvMatchPairs[i].first] == mvMatchPairs[i].second)
			{
				mvbInlierMask[i] = true;
			}
		}
	}
	int num_inlier = sum(mvbInlierMask)[0];
	return num_inlier;
}
