#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include<fstream>
using namespace std;
using namespace cv;
string argv[3];
int fps = 10; //֡��
int pict_num;
Mat image;
Mat img;
int SIZE0 = 7;//��λ��С��߳�
int SIZE1 = 7;//����С��߳�
int SIZE2 = 7 * SIZE0;//��λ�㷽��߳�
int SIZE3 = 72 * SIZE1;//��������߳�
int SIZE4 = SIZE0 * 18 + SIZE3;//����ͼƬ�߳����а�ɫ��Ȧ
int SIZE5 = SIZE4 + 40 * SIZE1;
ifstream ifs;
const int sublength = 42;
unsigned char* fileData = new unsigned char[sublength];
int col = 9 * SIZE0, row = SIZE0, c = 1;

void filePath()
{
	cout << "������������ļ�·��(ʹ��Ĭ��·��D:/pictures1/e1.bin����1)��" << endl;
	cin >> argv[1];
	if (argv[1][0] == '1')
		argv[1] = "D:/pictures1/e1.bin";
	cout << "��������Ƶ����ļ�(ʹ��Ĭ��·��D:/pictures1/����1)��" << endl;
	cin >> argv[2];
	if (argv[2][0] == '1')
		argv[2] = "D:/pictures1/";
}

void pictureNum()
{
	int time_limit;  //��Ƶ����,��λms,�û��Զ��壬��ʱ��ȡ
	cout << "������Ƶ����(��λms)��";
	cin >> time_limit;
	pict_num = (time_limit / 1000) * fps;
}

int* toBinary(int dec)
{
	int i, a[8] = { 0 };
	for (i = 0; dec > 0; dec /= 2, i++)
		a[i] = dec % 2; //�����������������,С����
	return a;
}

void creatImage()
{
	image = Mat(Size(SIZE5, SIZE4), CV_8UC1, Scalar(255));//�����̶���С��ɫͼƬ
	for (int i = 1; i < 17; i++)
		imwrite(argv[2] + "w" + to_string(i) + ".jpg", image);
	if (image.channels() > 1)
	{
		cvtColor(image, image, CV_RGB2GRAY);//תΪ��ͨ��
	}
	//����λ�㣬ͼƬ��0��ʼ�����ǻ�ͼ��1��ʼ
	for (int i = 1; i <= SIZE2; i++)
		for (int j = 1; j <= SIZE0; j++)
		{
			//����
			image.at<uchar>(i + SIZE0, j + SIZE0) = 0;//1�����ֱʻ�˳��
			image.at<uchar>(j + SIZE0, i + SIZE0) = 0;//2
			image.at<uchar>(SIZE0 + SIZE2 + 1 - i, SIZE0 + SIZE2 + 1 - j) = 0;//3
			image.at<uchar>(SIZE0 + SIZE2 + 1 - j, SIZE0 + SIZE2 + 1 - i) = 0;//4
			//����
			image.at<uchar>(i + SIZE4 - SIZE0 - SIZE2, j + SIZE0) = 0;//1�����ֱʻ�˳��
			image.at<uchar>(j + SIZE4 - SIZE0 - SIZE2, i + SIZE0) = 0;//2
			image.at<uchar>(SIZE4 - SIZE0 + 1 - i, SIZE0 + SIZE2 + 1 - j) = 0;//3
			image.at<uchar>(SIZE4 - SIZE0 + 1 - j, SIZE0 + SIZE2 + 1 - i) = 0;//4
			//����
			image.at<uchar>(i + SIZE0, j + SIZE5 - SIZE0 - SIZE2) = 0;//1�����ֱʻ�˳��
			image.at<uchar>(j + SIZE0, i + SIZE5 - SIZE0 - SIZE2) = 0;//2
			image.at<uchar>(SIZE0 + SIZE2 + 1 - i, SIZE5 - SIZE0 + 1 - j) = 0;//3
			image.at<uchar>(SIZE0 + SIZE2 + 1 - j, SIZE5 - SIZE0 + 1 - i) = 0;//4
			//����
			image.at<uchar>(i + SIZE4 - SIZE0 - SIZE2, j + SIZE5 - SIZE0 - SIZE2) = 0;//1�����ֱʻ�˳��
			image.at<uchar>(j + SIZE4 - SIZE0 - SIZE2, i + SIZE5 - SIZE0 - SIZE2) = 0;//2
			image.at<uchar>(SIZE4 - SIZE0 + 1 - i, SIZE5 - SIZE0 + 1 - j) = 0;//3
			image.at<uchar>(SIZE4 - SIZE0 + 1 - j, SIZE5 - SIZE0 + 1 - i) = 0;//4
		}
	//����λ���м�
	for (int i = 1; i <= 3 * SIZE0; i++)
		for (int j = 1; j <= 3 * SIZE0; j++)
		{
			image.at<uchar>(3 * SIZE0 + i, 3 * SIZE0 + j) = 0;
			image.at<uchar>(SIZE4 - 6 * SIZE0 + i, 3 * SIZE0 + j) = 0;
			image.at<uchar>(3 * SIZE0 + i, SIZE5 - 6 * SIZE0 + j) = 0;
			image.at<uchar>(SIZE4 - 6 * SIZE0 + i, SIZE5 - 6 * SIZE0 + j) = 0;
		}
	/*//���ͼƬ���
	int num[8];
	memcpy(num, toBinary(c), sizeof(num));
	for (int j = 0; j < 8; j++)
	{
		if (num[j] == 1)
		{
			//�ı����ؿ�Ϊ��ɫ
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
	unsigned int buf[8];//У��ֵ������
	memcpy(buf, toBinary(crc), sizeof(buf));
	for (int j = 0; j < 8; j++)
	{
		if (buf[j] == 1)
		{
			//�ı����ؿ�Ϊ��ɫ
			for (int m = 1; m <= SIZE1; m++)
				for (int n = 1; n <= SIZE1; n++)
					img.at<uchar>(row + m, col + n) = 0;
		}
		col += SIZE1;
	}
}

void saveImg(int i)
{
	//���屣��ͼ�������
	string strSaveName;
	strSaveName = to_string(i);

	//���屣��ͼ�������·��
	string strImgSavePath = argv[2] + strSaveName;
	//���屣��ͼ��ĸ�ʽ
	strImgSavePath += ".jpg";
	//�������
	imwrite(strImgSavePath.c_str(), img);
}

//�����ж�
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
		else //����һ��ͼ
		{
			row = SIZE0;//��ʼ������
			col = 9 * SIZE0;
			saveImg(c);
			++c;//ͼƬ��������
			img = image.clone();//�µĿհ�ͼ
		}
	}
}

void creatImg()
{
	binaryRead();//��fileData���鸳ֵ
	int binData[8];//��ÿ���ַ���8λ������
	creatCRC();
	for (int i = 0; i < sublength; ++i)//����Դ�ļ��е��ַ�
	{
		Newline();
		memcpy(binData, toBinary(fileData[i]), sizeof(binData));//��binData������ڶ���������
		for (int k = 0; k < 8; ++k)
		{
			//�ı����ؿ�Ϊ��ɫ
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
	size_t count = img.size();  //ͼƬ��Ŀ
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
		cout << "�ļ���ʧ��" << endl;
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