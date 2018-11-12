#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"

#pragma comment(lib, "gdal_i.lib")

int main()
{
	GDALDataset * poSrcDS;
	GDALDataset * poDstDS;

	int imgXlen, imgYlen;
	int StartX = 100, StartY = 100, tempXlen = 200, tempYlen = 150;


	char * srcPath = "yiyiyi.jpg";
	char * dstPath = "dst.tif";

	GByte * buffTmp; // 图像储存

	int i, bandNum;

	//注册驱动

	GDALAllRegister();

	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath, GA_ReadOnly);

	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	buffTmp = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));

	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
	

	
	for (i = 0; i < bandNum; i++)
	{
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	

	/*
	for (int k = 0; k < 3; k++)
	{

		poSrcDS->GetRasterBand(k+1)->RasterIO(GF_Read, StartX, StartY, tempXlen, tempYlen, buffTmp, tempXlen, tempYlen, GDT_Byte, 0, 0);
		
		for (int j = 0; j < tempYlen; j++)
		for (int i = 0; i < tempXlen; i++)
		{
			buffTmp[j*tempXlen + i] = (GByte)255;
		}
		poDstDS->GetRasterBand(k+1)->RasterIO(GF_Write, StartX, StartY, tempXlen, tempYlen, buffTmp, tempXlen, tempYlen, GDT_Byte, 0, 0);

	}*/
	

	/***********************************作业部分**********************************/
	for (int k = 0; k < bandNum; k++)
	{

		poSrcDS->GetRasterBand(k + 1)->RasterIO(GF_Read, StartX, StartY, tempXlen, tempYlen, buffTmp, tempXlen, tempYlen, GDT_Byte, 0, 0);


		for (int j = 0; j < tempYlen; j++)
		for (int i = 0; i < tempXlen; i++)
		{
			buffTmp[j*tempXlen + i] = (GByte)255;
		}
		poDstDS->GetRasterBand(k + 1)->RasterIO(GF_Write, StartX, StartY, tempXlen, tempYlen, buffTmp, tempXlen, tempYlen, GDT_Byte, 0, 0);

	}
	



	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
}