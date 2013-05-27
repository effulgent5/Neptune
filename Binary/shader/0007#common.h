#define CHL_SKINNED		0x00000001

#define TEST_FLAG(flag, chl)	\
	(((flag / chl) / 2) != ((flag / chl + 1) / 2))

cbuffer cbPreFrame : register(b0)
{
	float4x4	g_view : packoffset(c0);
	float4x4	g_proj : packoffset(c4);
};

cbuffer cbPreObject : register(b1)
{
	float4x4	g_world : packoffset(c0);
};

