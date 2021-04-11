#include <stdint.h>
#include <uchar.h>

#ifndef nds_h
#define nds_h

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef struct nds_header //the NDS file header
{
	char game_title[12]; //the game title
	char game_code[4]; //the game product code
	char maker_code[2]; //the code identifying the maker
	u8 unit_code; //identifier of console 
	u8 enc_seed_select; //???
	u8 device_capacity; //??
	u8 reserved[9]; //??
	u8 rom_version; //ROM version
	u8 internal_flags; //flags
	u32 arm9_rom_offset;
	u32 arm9_entry_address;
	u32 arm9_load_address;
	u32 arm9_size;
	u32 arm7_rom_offset;
	u32 arm7_entry_address;
	u32 arm7_load_address;
	u32 arm7_size;
	u32 fnt_offset;
	u32 fnt_size;
	u32 fat_offset;
	u32 fat_size;
	u32 arm9_overlay_offset;
	u32 arm9_overlay_size;
	u32 arm7_overlay_offset;
	u32 arm7_overlay_size;
	u32 normal_card_ctrl_reg_settings;
	u32 secure_card_ctrl_reg_settings;
	u32 icon_banner_offset; //the important part here
	u16 secure_area_crc;
	u16 secure_transfer_timeout;
	u32 arm9_autoload;
	u32 arm7_autoload;
	u8 secure_disable[8];
	u32 ntr_region_rom_size;
	u32 header_size;
	u8 reserved_2[56];
	u8 nintendo_logo[156];
	u16 nintendo_logo_crc;
	u16 header_crc;
	u8 reserved_debugger[32];

} nds_header;

typedef struct nds_icon_banner
{
	u8 num_titles;
	u8 animated;
	u16 checksums[4];
	u8 padding[22];
	u8 default_bitmap[512];
	u16 default_bitmap_palette[0x10];
	u8 titles[256][6];
	//since this program is only meant to work with NDS roms and not DSi NDS, we stop here
	
} nds_icon_banner;

#endif