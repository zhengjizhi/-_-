#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"

#pragma comment(lib, "gdal_i.lib")

void convolution(int order, float *matrix, char * inputFileName, char * outputFileName, float multiple);

int main()
{
	float matrix1[9] = 
	{
		0, 1, 0,
		1, 1, 1,
		0, 1, 0
	};
	float matrix2[25] = 
	{
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0,
		0, 0, 0, 0, 1 
	};
	float matrix3[9] =
	{
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	
	float matrix4[9] =
	{
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	};

	float matrix5[9] =
	{
		-1, -1, 0,
		-1,  0, 1,
		 0,  1, 1
	};

	float matrix6[25] =
	{
		0.0120, 0.1253, 0.2736, 0.1253, 0.0120,
		0.1253, 1.3054, 2.8514, 1.3054, 0.1253,
		0.2736, 2.8514, 6.2279, 2.8514, 0.2736,
		0.1253, 1.3054, 2.8514, 1.3054, 0.1253,
		0.0120, 0.1253, 0.2736, 0.1253, 0.0120
	};

	convolution(3, matrix1, "lena.jpg", "output1.tif",0.2);
	convolution(5, matrix2, "lena.jpg", "output2.tif",0.2);
	convolution(3, matrix3, "lena.jpg", "output3.tif", 1);
	convolution(3, matrix4, "lena.jpg", "output4.tif", 1);
	convolution(3, matrix5, "lena.jpg", "output5.tif", 1);
	convolution(5, matrix6, "lena.jpg", "output6.tif", 0.04);

	system("pause");


	return 0;
}


void convolution(int order, float *matrix, char * inputFileName, char * outputFileName, float multiple)
{
	GDALAllRegister();

	GDALDataset * poSrcDS;

	GDALDataset * poDstDS;

	GByte * buffTmp, *buffTmp_;

	poSrcDS = (GDALDataset *)GDALOpenShared(inputFileName, GA_ReadOnly);

	int imgXlen = poSrcDS->GetRasterXSize();
	int imgYlen = poSrcDS->GetRasterYSize();
	int bandNum = poSrcDS->GetRasterCount();
	float tempP;


	int offset = order / 2;

	buffTmp = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
	buffTmp_ = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));



	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outputFileName, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);


	for (int i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp_, imgXlen, imgYlen, GDT_Byte, 0, 0);

		for (int y = offset; y <= imgYlen - offset; y++)
		for (int x = offset; x < imgXlen - offset; x++)
		{
			//buffTmp_[y * imgXlen + x] = (buffTmp[(y)*imgXlen + x] + buffTmp[(y - 1)*imgXlen + x] + buffTmp[(y + 1)*imgXlen + x] + buffTmp[(y)*imgXlen + x - 1] + buffTmp[(y)*imgXlen + x + 1]) *0.2;
			
			tempP = 0;
			for (int oy = offset * -1; oy <= offset; oy++)
			for (int ox = offset * -1; ox <= offset; ox++)
			{
				tempP += buffTmp[(y + oy) * imgXlen + x + ox] * matrix[(oy + offset)*order + ox + offset];
				//cout <<"y:" <<y <<endl<<"x:"<< x <<endl<<"y + oy:"<< (y + oy)<<endl <<"x + ox:" << x + ox <<endl<<"oy + offset:"<< oy + offset <<endl<<"ox + offset" << ox + offset<<endl;
				//cout << (int)buffTmp_[y * imgXlen + x] << endl;
				//cout << (int)buffTmp[(y + oy)* imgXlen + x + ox] << endl << matrix[(oy + offset)*order + ox + offset] << endl << buffTmp[(y + oy) * imgXlen + x + ox] * matrix[(oy + offset)*order + ox + offset] << endl;
				//system("pause");
			}
			//cout << (int)buffTmp_[y * imgXlen + x] << endl ;
			buffTmp_[y * imgXlen + x] = tempP*multiple;
			//cout << (int)buffTmp_[y * imgXlen + x] << endl;
			//if (x == 50) system("pause");
		}

		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp_, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}


	GDALClose(poDstDS);
	GDALClose(poSrcDS);
	CPLFree(buffTmp);
	CPLFree(buffTmp_);
}