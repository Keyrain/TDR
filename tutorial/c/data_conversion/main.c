#include <stdio.h>
#include "hotpot/hp_platform.h"
#include "proto_types.h"
#include "hotprotocol/hp_xml_reader.h"
#include "proto_reader.h"
#include "proto_writer.h"

#include "hotprotocol/hp_compact_reader.h"
#include "hotprotocol/hp_compact_writer.h"

#include <string.h>

#define MAX_BUFF_SIZE 1024

PROTO proto;
char buff[65536];
hpuint32 buff_len;

void test_xml()
{
	HP_XML_READER xml_reader;
	FILE *fin = fopen("d:/t.xml", "rb");

	xml_reader_init(&xml_reader, fin);
	read_PROTO(&xml_reader.super, &proto);
	fclose(fin);
}

void test_compact()
{
	HP_COMPACT_READER compact_reader;
	HP_COMPACT_WRITER compact_writer;

	hp_compact_writer_init(&compact_writer, buff, sizeof(buff));

	proto.pos[0].id = 1;
	proto.pos[0].lat = 123.321;
	proto.pos[0].lng = 789.987;
	proto.pos_size = 1;
	proto.id = 222;
	snprintf(proto.name, sizeof(buff), "randyliu");

	write_PROTO(&compact_writer.super, &proto);
	memset(&proto, 0, sizeof(proto));

	hp_compact_reader_init(&compact_reader, buff, sizeof(buff));
	read_PROTO(&compact_reader.super, &proto);
	
}

int main()
{
	//test_xml();

	test_compact();

	return 0;
}
