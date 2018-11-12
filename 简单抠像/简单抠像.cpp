#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"

#pragma comment(lib, "gdal_i.lib")

int main()
{

	GDALDataset * poSrcDS;
	GDALDataset * poSrcDS1;


	GDALDataset * poDstDS;

	int imgXlen, imgYlen;

	char * srcPath = "superMan.jpg";

	char * dstPath = "synthesis.tif";

	char * spacePath = "space.jpg";

	GByte * spacebuffTmpR;//Í¼ÏñÄÚ´æ´æ´¢
	GByte * spacebuffTmpG;
	GByte * spacebuffTmpB;

	GByte * superMbuffTmpR;
	GByte * superMbuffTmpG;
	GByte * superMbuffTmpB;

	int i, bandNum;


	//×¢²áÇý¶¯

	GDALAllRegister();

	

	poSrcDS = (GDALDataset *)GDALOpenShared(spacePath, GA_ReadOnly);

	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();


	spacebuffTmpR = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
	spacebuffTmpG = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
	spacebuffTmpB = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));

	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, spacebuffTmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDS->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, spacebuffTmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDS->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, spacebuffTmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);

	poSrcDS1 = (GDALDataset *)GDALOpenShared(srcPath, GA_ReadOnly);

	superMbuffTmpR = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
	superMbuffTmpG = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
	superMbuffTmpB = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));

	poSrcDS1->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, superMbuffTmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDS1->GetRasterBand(2)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, superMbuffTmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poSrcDS1->GetRasterBand(3)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, superMbuffTmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);


	for (int y = 0; y < imgYlen; y++)
	for (int x = 0; x < imgXlen; x++)
	{
		if (!(superMbuffTmpR[y*imgXlen + x] > 10 && superMbuffTmpR[y*imgXlen + x] < 160 && superMbuffTmpG[y*imgXlen + x] > 100 && superMbuffTmpG[y*imgXlen + x] < 220 && superMbuffTmpB[y*imgXlen + x] > 10 && superMbuffTmpB[y*imgXlen + x] < 110))
		{
			spacebuffTmpR[y*imgXlen + x] = superMbuffTmpR[y*imgXlen + x];
			spacebuffTmpG[y*imgXlen + x] = superMbuffTmpG[y*imgXlen + x];
			spacebuffTmpB[y*imgXlen + x] = superMbuffTmpB[y*imgXlen + x];
		}
	}

	poDstDS->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, spacebuffTmpR, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, spacebuffTmpG, imgXlen, imgYlen, GDT_Byte, 0, 0);
	poDstDS->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, spacebuffTmpB, imgXlen, imgYlen, GDT_Byte, 0, 0);


	CPLFree(spacebuffTmpR);
	CPLFree(spacebuffTmpG);
	CPLFree(spacebuffTmpB);
	CPLFree(superMbuffTmpR);
	CPLFree(superMbuffTmpG);
	CPLFree(superMbuffTmpB);
	GDALClose(poDstDS);
	GDALClose(poSrcDS1);


	system("pause");


	return 0;
}