#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"

#pragma comment(lib, "gdal_i.lib")

int main()
{
	//输入图像
	GDALDataset * poSrcDS;
	//输出图像
	GDALDataset * poDstDS;

	//图像的长宽
	int imgXlen,imgYlen;

	//输入 输出图像路径
	char * srcPath = "yiyiyi.jpg";
	char * dstPath = "res.tif";

	GByte * buffTmp;//图像内存存储

	int i,bandNum;


	//注册驱动

	GDALAllRegister();

	//打开图像
	poSrcDS = (GDALDataset *)GDALOpenShared(srcPath,GA_ReadOnly);


	//获取图像的长宽 和 波段数
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();

	cout << "Image x Length : " << imgXlen << endl;
	cout << "Image y Length : " << imgYlen << endl;
	cout << "Band number    : " << bandNum << endl;

	//分配内存
	buffTmp = (GByte *)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));

	//创建输出图像
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath,imgXlen,imgYlen,bandNum,GDT_Byte,NULL);

	//一个个波段的存到bufftmp 一个个的从bufftmp存到输出图像里边  此时的bufftmp存着像素值
	for( i = 0; i < bandNum ; i++ )
	{

		poSrcDS->GetRasterBand(i+1)->RasterIO(GF_Read,0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);

		poDstDS->GetRasterBand(i+1)->RasterIO(GF_Write,0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);

		printf("... ... band %d processing ... ...\n",i);
	}

	//释放内存
	CPLFree(buffTmp);

	//关闭
	GDALClose(poDstDS);
	GDALClose(poSrcDS);


	system("pause");


	return 0;
}