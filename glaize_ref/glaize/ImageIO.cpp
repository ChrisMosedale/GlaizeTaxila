#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <vector>

#include "ImageIO.h"

using namespace std;


string ImageIO::csvf = cfg.input_csv_file;


string ImageIO::itos(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}


string ImageIO::ftos(double d)
{
	stringstream ss;
	ss << std::fixed;
	ss << std::setprecision(1);
	ss << d;
	return ss.str();
}

int ImageIO::last_run_file_id(string f)
{
	size_t i, j;
	j = f.find_last_of('.');
	string stub = f.substr(0, j);
	i = stub.find_last_not_of("0123456789");
	string ids = stub.substr(i);
	return config::strtoi(ids);
}


ImageIO::ImageIO()
	:num_fingers(4), working_dir("")
{
	csvf = cfg.input_csv_file;
	try
	{
		_csvfs.open(csvf, ios_base::in);
	}
	catch (...)
	{
		throw invalid_argument(string("[ImageIO]  Failed to open image input file ") + csvf);
	}
}


string ImageIO::getLeftFingerF()
{
	size_t idx;
	string lff, file;

	getline(_csvfs, lff, ',');
	while (lff[0] == '#')
	{
		getline(_csvfs, lff);
		getline(_csvfs, lff, ',');
	}

	num_fingers = (size_t)(config::strtoi(lff));

	getline(_csvfs, file, ',');
	string path = file;

	idx = path.find("ref_nails");
	path.replace(idx, 9, "testref");
	working_dir = path;

	getline(_csvfs, lff );
	file = file + lff;

	idx = lff.find_last_of(".");
	_working_fn = lff.substr(0, idx);

	return file;
}


string ImageIO::getLeftThumbF()
{
	string f;
	getline(_csvfs, f);
	size_t end = f.find("_image.png");
	string fn = f.substr(0, end);
	return fn;
}


string ImageIO::getFingerMask(uint8_t fid) const
{
	string ids = (num_fingers == 4) ? ImageIO::itos(fid) : ImageIO::itos(4);
	string f = working_dir + _working_fn + "_l" + ids + ".png";
	return f;
}


string ImageIO::getCsvFile() const
{
	string temp(_working_fn);
	temp.replace( 0, 5, "");

	string f = cfg.results_csv_dir + temp + ".csv";
	return f;
}


void ImageIO::output_csv(vector< pair< vector<float>, vector<float> > >& nail_metrics)
{
	string csvf = getCsvFile();

	cout << "outputting to: " << csvf;

	ofstream ofs(csvf.c_str(), ios_base::out);
	ofs << std::fixed;
	ofs << std::setprecision(1);
	ofs << "finger ID from left small finger, left half (l) or right half (r), "
		<< "half cross section mm at 0.5mm intervals from bottom up... " << endl;
	for (size_t i = 0; i < GL_NUM_FINGERS; ++i)
	{
		ofs << i << ",l,";
		for (size_t j = 0; j < nail_metrics[i].first.size(); ++j)
		{
			ofs << nail_metrics[i].first[j] << ",";
		}
		ofs << endl;

		ofs << i << ",r,";
		for (size_t j = 0; j < nail_metrics[i].second.size(); ++j)
		{
			ofs << nail_metrics[i].second[j] << ",";
		}
		ofs << endl;
	}
}

