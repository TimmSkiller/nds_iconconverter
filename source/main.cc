#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>

#include "bmp.hh"
#include "nds.hh"

int main(int argc, char *argv[])
{
	//create the bitmap, make sure that it is set to 00
	BMP<32, 32, 16> bmp;

	memset((&bmp)->data, 0x00, bmp_data_size(bmp));

	if (argc < 2) {
		std::cout << "No NDS file was specified!";
		return -1;
	}

	std::string path = std::string(argv[1]);

	struct stat *stat_file = new struct stat;

	if((stat(argv[1], stat_file)) == 0)
	{
		std::cout << "file at " << path << ":\n" << "size: " << std::to_string(stat_file->st_size) << "\n";
	}
	else
	{
		std::cout << "failed to stat file at " << path << "\n" << "errno: " << std::to_string(errno) << "\n";

		delete stat_file;

		return -1;
	}

	FILE *nds = fopen(argv[1], "r"); //open the input file for reading

	nds_header *header = new nds_header; //read the header

	if (!fread(header, sizeof(char), sizeof(nds_header), nds))
	{
		std::cout << "could not read header from nds\n";
		
		delete header;
		delete stat_file;

		fclose(nds);

		return -1;
	}

	std::cout << "Read NDS rom with title: \"" << std::string(header->game_title, 12) << "\" and game code: \"" << std::string(header->game_code, 4) << "\"\n";
	std::cout << "Icon banner offset: 0x" << std::hex << header->icon_banner_offset << "\n";
	
	fseek(nds, header->icon_banner_offset, SEEK_SET);

	nds_icon_banner *banner = new nds_icon_banner;

	if (!fread(banner, sizeof(char), sizeof(nds_icon_banner), nds))
	{
		std::cout << "Failed reading icon banner from NDS File!\n";

		delete header;
		delete banner;
		delete stat_file;

		fclose(nds);

		return -1;
	}

	fclose(nds); //nds is no longer needed, close it

	// taken from desmume: https://github.com/TASVideos/desmume/blob/85a2674eb5894511676603ea0d152dff29baa064/desmume/src/frontend/windows/ginfo.cpp#L224-L302

	for(u32 y = 0; y < 32; y++)
	{
		for(u32 x = 0; x < 32; x++)
		{
			int tilenum = (((y / 8) * 4) + (x / 8));
			int tilex = (x % 8);
			int tiley = (y % 8);
			int mapoffset = ((tilenum * 64) + (tiley * 8) + tilex);

			u8 val = banner->default_bitmap[(mapoffset >> 1)];

			val = (mapoffset & 1) ? ((val >> 4) & 0xF) : (val & 0xF);

			bmp.data[(y * 32) + x] = banner->default_bitmap_palette[val];
		}
	}

	//vertically flip the data

	u16 *vflip = new u16[1024];

	int index = 0;

	for (int i = 1024; i > 0; i -= 32)
	{
		memcpy(vflip + index, bmp.data + i - 32, 64);

		index += 32;
	}

	memcpy(bmp.data, vflip, 2048);

	delete [] vflip;

	std::string out_path = path + ".icon.bmp";

	save_to_file(bmp, out_path.c_str());

	std::cout << "BMP was written successfully!" << std::endl;

	delete header;
	delete banner;
	delete stat_file;

	return 0;
}