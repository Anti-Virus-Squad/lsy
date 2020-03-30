#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include<fstream>
using namespace std;
using namespace cv;
string argv[3];
int fps = 10; //帧数
int pict_num;
Mat image;
Mat img;
int SIZE0 = 7;//定位点小格边长
int SIZE1 = 7;//数据小格边长
int SIZE2 = 7 * SIZE0;//定位点方块边长
int SIZE3 = 72 * SIZE1;//数据区域边长
int SIZE4 = SIZE0 * 18 + SIZE3;//整张图片边长，有白色外圈
int SIZE5 = SIZE4 + 40 * SIZE1;
ifstream ifs;
const int sublength = 42;
unsigned char* fileData = new unsigned char[sublength];
int col = 9 * SIZE0, row = SIZE0, c = 1;

void filePath()
{
	cout << "请输入二进制文件路径(使用默认路径D:/pictures1/e1.bin输入1)：" << endl;
	cin >> argv[1];
	if (argv[1][0] == '1')
		argv[1] = "D:/pictures1/e1.bin";
	cout << "请输入视频输出文件(使用默认路径D:/pictures1/输入1)：" << endl;
	cin >> argv[2];
	if (argv[2][0] == '1')
		argv[2] = "D:/pictures1/";
}

void pictureNum()
{
	int time_limit;  //视频长度,单位ms,用户自定义，到时读取
	cout << "生成视频长度(单位ms)：";
	cin >> time_limit;
	pict_num = (time_limit / 1000) * fps;
}

int* toBinary(int dec)
{
	int i, a[8] = { 0 };
	for (i = 0; dec > 0; dec /= 2, i++)
		a[i] = dec % 2; //将余数正向存入数组,小端序
	return a;
}

void creatImage()
{
	image = Mat(Size(SIZE5, SIZE4), CV_8UC1, Scalar(255));//创建固定大小白色图片
	for (int i = 1; i < 17; i++)
		imwrite(argv[2] + "w" + to_string(i) + ".jpg", image);
	if (image.channels() > 1)
	{
		cvtColor(image, image, CV_RGB2GRAY);//转为单通道
	}
	//画定位点，图片从0开始，但是画图从1开始
	for (int i = 1; i <= SIZE2; i++)
		for (int j = 1; j <= SIZE0; j++)
		{
			//左上
			image.at<uchar>(i + SIZE0, j + SIZE0) = 0;//1，口字笔画顺序
			image.at<uchar>(j + SIZE0, i + SIZE0) = 0;//2
			image.at<uchar>(SIZE0 + SIZE2 + 1 - i, SIZE0 + SIZE2 + 1 - j) = 0;//3
			image.at<uchar>(SIZE0 + SIZE2 + 1 - j, SIZE0 + SIZE2 + 1 - i) = 0;//4
			//左下
			image.at<uchar>(i + SIZE4 - SIZE0 - SIZE2, j + SIZE0) = 0;//1，口字笔画顺序
			image.at<uchar>(j + SIZE4 - SIZE0 - SIZE2, i + SIZE0) = 0;//2
			image.at<uchar>(SIZE4 - SIZE0 + 1 - i, SIZE0 + SIZE2 + 1 - j) = 0;//3
			image.at<uchar>(SIZE4 - SIZE0 + 1 - j, SIZE0 + SIZE2 + 1 - i) = 0;//4
			//右上
			image.at<uchar>(i + SIZE0, j + SIZE5 - SIZE0 - SIZE2) = 0;//1，口字笔画顺序
			image.at<uchar>(j + SIZE0, i + SIZE5 - SIZE0 - SIZE2) = 0;//2
			image.at<uchar>(SIZE0 + SIZE2 + 1 - i, SIZE5 - SIZE0 + 1 - j) = 0;//3
			image.at<uchar>(SIZE0 + SIZE2 + 1 - j, SIZE5 - SIZE0 + 1 - i) = 0;//4
			//右下
			image.at<uchar>(i + SIZE4 - SIZE0 - SIZE2, j + SIZE5 - SIZE0 - SIZE2) = 0;//1，口字笔画顺序
			image.at<uchar>(j + SIZE4 - SIZE0 - SIZE2, i + SIZE5 - SIZE0 - SIZE2) = 0;//2
			image.at<uchar>(SIZE4 - SIZE0 + 1 - i, SIZE5 - SIZE0 + 1 - j) = 0;//3
			image.at<uchar>(SIZE4 - SIZE0 + 1 - j, SIZE5 - SIZE0 + 1 - i) = 0;//4
		}
	//画定位点中间
	for (int i = 1; i <= 3 * SIZE0; i++)
		for (int j = 1; j <= 3 * SIZE0; j++)
		{
			image.at<uchar>(3 * SIZE0 + i, 3 * SIZE0 + j) = 0;
			image.at<uchar>(SIZE4 - 6 * SIZE0 + i, 3 * SIZE0 + j) = 0;
			image.at<uchar>(3 * SIZE0 + i, SIZE5 - 6 * SIZE0 + j) = 0;
			image.at<uchar>(SIZE4 - 6 * SIZE0 + i, SIZE5 - 6 * SIZE0 + j) = 0;
		}
	/*//添加图片编号
	int num[8];
	memcpy(num, toBinary(c), sizeof(num));
	for (int j = 0; j < 8; j++)
	{
		if (num[j] == 1)
		{
			//改变像素块为黑色
			for (int m = 1; m <= SIZE1; m++)
				for (int n = 1; n <= SIZE1; n++)
					img.at<uchar>(row + m, col + n) = 0;
		}
		col += SIZE1;
	}*/
}

void binaryRead() {
	unsigned char c;
	for (int i = 0; i < sublength; i++)
	{
		ifs.read((char*)&c, sizeof(c));
		fileData[i] = c;
	}
}

unsigned char CRC8(unsigned char* data, unsigned int datalen)

{

	unsigned char wCRCin = 0x00;

	unsigned char wCPoly = 0x07;



	while (datalen--)

	{

		wCRCin ^= *(data++);

		for (int i = 0; i < 8; i++)

		{

			if (wCRCin & 0x80)

				wCRCin = (wCRCin << 1) ^ wCPoly;

			else

				wCRCin = wCRCin << 1;

		}

	}

	return (wCRCin);

}

void creatCRC()
{
	unsigned char crc;
	crc = CRC8(fileData, sublength);
	unsigned int buf[8];//校验值二进制
	memcpy(buf, toBinary(crc), sizeof(buf));
	for (int j = 0; j < 8; j++)
	{
		if (buf[j] == 1)
		{
			//改变像素块为黑色
			for (int m = 1; m <= SIZE1; m++)
				for (int n = 1; n <= SIZE1; n++)
					img.at<uchar>(row + m, col + n) = 0;
		}
		col += SIZE1;
	}
}

void saveImg(int i)
{
	//定义保存图像的名字
	string strSaveName;
	strSaveName = to_string(i);

	//定义保存图像的完整路径
	string strImgSavePath = argv[2] + strSaveName;
	//定义保存图像的格式
	strImgSavePath += ".jpg";
	//保存操作
	imwrite(strImgSavePath.c_str(), img);
}

//换行判断
void Newline()
{
	if (col == SIZE5 - 9 * SIZE0 || col == SIZE5 - SIZE0)
	{
		if (row < 9 * SIZE0 - SIZE1)
		{
			col = 9 * SIZE0;
			row += SIZE1;
		}
		else if (row == 9 * SIZE0 - SIZE1)
		{
			col = SIZE0;
			row += SIZE1;
		}
		else if (row < SIZE4 - 9 * SIZE0 - SIZE1)
		{
			if (col == SIZE5 - SIZE0)
			{
				col = SIZE0;
				row += SIZE1;
			}
		}
		else if (row == SIZE4 - 9 * SIZE0 - SIZE1)
		{
			if (col == SIZE5 - SIZE0)
			{
				col = 9 * SIZE0;
				row += SIZE1;
			}
		}
		else if (row < SIZE4 - SIZE0 - SIZE1)
		{
			col = 9 * SIZE0;
			row += SIZE1;
		}
		else //用完一张图
		{
			row = SIZE0;//初始化坐标
			col = 9 * SIZE0;
			saveImg(c);
			++c;//图片编码增加
			img = image.clone();//新的空白图
		}
	}
}

void creatImg()
{
	binaryRead();//给fileData数组赋值
	int binData[8];//放每个字符的8位二进制
	creatCRC();
	for (int i = 0; i < sublength; ++i)//控制源文件中的字符
	{
		Newline();
		memcpy(binData, toBinary(fileData[i]), sizeof(binData));//令binData数组等于二进制数组
		for (int k = 0; k < 8; ++k)
		{
			//改变像素块为黑色
			if (binData[k] == 1)
			{
				for (int m = 1; m <= SIZE1; m++)
					for (int n = 1; n <= SIZE1; n++)
						img.at<uchar>(row + m, col + n) = 0;
			}
			col += SIZE1;
		}
	}
	Newline();
}

void creatVideo()
{
	vector<String> img;
	glob(argv[2], img, false);
	size_t count = img.size();  //图片数目
	VideoWriter video(argv[2] + "videoW.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(910, 630));
	VideoWriter truevideo(argv[2] + "video.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(910, 630));
	Mat Image;
	for (int i = 1; i < 7; i++)
	{
		stringstream str;
		str << "w" << i << ".jpg";
		Image = imread(argv[2] + str.str());
		resize(Image, Image, Size(910, 630));
		video << Image;
	}
	for (size_t i = 1; i <= count; i++)
	{
		stringstream str;
		str << i << ".jpg";
		Image = imread(argv[2] + str.str());
		if (!Image.empty())
		{
			resize(Image, Image, Size(910, 630));
			video << Image;
			truevideo << Image;
		}
	}
	for (int i = 7; i < 17; i++)
	{
		stringstream str;
		str << "w" << i << ".jpg";
		Image = imread(argv[2] + str.str());
		resize(Image, Image, Size(910, 630));
		video << Image;
	}
}

int main()
{
	filePath();
	pictureNum();
	creatImage();
	ifs.open(argv[1], ios::in | ios::binary);
	if (!ifs.is_open()) {
		cout << "文件打开失败" << endl;
		exit(0);
	}
	img = image.clone();
	creatImg();
	while (c <= pict_num && !ifs.eof())
	{
		creatImg();
	}
	creatVideo();
	delete fileData;
	ifs.close();
	image.release();
	img.release();
	return 0;
}