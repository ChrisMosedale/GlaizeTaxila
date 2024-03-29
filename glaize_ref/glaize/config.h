#pragma once

#include <fstream>

#define GLAIZE_CFG_FILE			"C:/Apps/glaize/glaize_config.txt"

#define GL_DATA_DIR				"D:/data/"
#define GL_DATA_WORKING_SUBDIR	"testref/"
#define GL_IMAGE_INPUT_CSV		"rec.csv"

#define GL_RESULTS_PATH			"results/csv/"	

#define CC_LEN_PX				856
#define CC_PX_PER_MM			10

#define GL_NUM_FINGERS			5



using namespace std;


class config
{
public:

	config();

	string data_dir;
	string working_dir;
	string input_csv_file;
	string results_csv_dir;

	int last_run_file_id;

	static int strtoi(string s);

private:

	string	_cfgFile;
	ifstream _cfgfs;

};

extern config cfg;

