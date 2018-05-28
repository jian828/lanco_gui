
#include "inc/app_def.h"




const unsigned char  b64str[] = 
{ 
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
	0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,
	0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64,
	0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
	0x38, 0x39, 0x2d, 0x2a
};




/// [jjhii]  Modified so it no longer append = to a value.
int base64_encode (const unsigned char *read, unsigned short inlen, unsigned char *out, unsigned short outlen)
{
	int len = 0;
	unsigned char ch, in[3];
	while (inlen && outlen)
	{
		/// read data from flash

		in[0] = read[0];
		in[1] = read[1];
		in[2] = read[2];

		ch = b64str[((in[0]) >> 2) & 0x3f];
		//set_unsigned char(ch, out++); // *out++ = ch;
		*out++ = ch;
		len++;
		if (!--outlen)
			break;
		ch = b64str[(((in[0]) << 4) + (--inlen ? (in[1]) >> 4 : 0)) & 0x3f]; 
		//set_unsigned char(ch, out++); // *out++ = ch;
		*out++ = ch;
		len++;
		if (!--outlen)
			break;
		ch = (inlen	? b64str[(((in[1]) << 2) + (--inlen ? (in[2]) >> 6 : 0)) & 0x3f] : 0x0000);
		//set_unsigned char(ch, out++); // *out++ = ch;
		*out++ = ch;
		if(ch != 0x0000)
			len++;
		if (!--outlen)
			break;
		ch = inlen ? b64str[(in[2]) & 0x3f] : 0x0000; 
		//set_unsigned char(ch, out++); // *out++ = ch;
		*out++ = ch;
		if(ch != 0x0000)
			len++;
		if (!--outlen)
			break;
		if (inlen)
			inlen--;
		if (inlen)
			read += 3;
	}

	if (outlen)
		//set_unsigned char(ch, out++); // *out = 0x0000;
		*out = 0x0000;

	return len;
}

int isbase64(unsigned char ch)
{
	return (ch >= 0x41 && ch <= 0x5a) || \
		(ch >= 0x61 && ch <= 0x7a) || \
		(ch >= 0x30 && ch <= 0x39) || \
		(ch == 0x2d) || (ch == 0x2a);
}

unsigned char b64(unsigned char ch)
{
	if (ch >= 0x41 && ch <= 0x5a)
		return ch - 0x41;
	if (ch >= 0x61 && ch <= 0x7a)
		return (ch - 0x61) + 26;
	if (ch >= 0x30 && ch <= 0x39)
		return (ch - 0x30) + 52;
	if (ch == 0x2d)
		return 62;
	if (ch == 0x2a)
		return 63;

	return 0;
}



int base64_decode (const unsigned char *read, unsigned short inlen, unsigned char *out, unsigned short *outlen)
{
	int outleft = *outlen;
	unsigned char ch, in[4];

	while (inlen >= 2)
	{
		/// read data from flash
		in[0] = read[0];
		in[1] = read[1];
		in[2] = read[2];
		in[3] = read[3];
		
		if (!isbase64 (in[0]) || !isbase64 (in[1]))
			break;

		if (outleft)
		{
			ch = ((b64((in[0])) << 2) | (b64((in[1])) >> 4));
			*out++ = ch;
			outleft--;
		}

		if (inlen == 2)
		{
			inlen -= 2; // force a valid response [jjhii]
			break;
		}

		if (in[2] == 0x3d)
		{
			if (inlen != 4)
				break;

			if (in[3] != 0x3d)
				break;
		}
		else
		{
			if (!isbase64 (in[2]))
				break;

			if (outleft)
			{
				ch = (((b64((in[1])) << 4) & 0xf0) | (b64((in[2])) >> 2));
				*out++ = ch;
				outleft--;
			}

			if (inlen == 3)
			{
				inlen -= 3; // force a valid response [jjhii]
				break;
			}

			if (in[3] == 0x3d)
			{
				if (inlen != 4)
					break;
			}
			else
			{
				if (!isbase64 (in[3]))
					break;

				if (outleft)
				{
					ch = (((b64((in[2])) << 6) & 0xc0) | b64((in[3])));
					*out++ = ch;
					outleft--;
				}
			}
		}

		read += 4;
		inlen -= 4;
	}

	*outlen -= outleft;

	if (inlen != 0)
		return 0;

	return 1;
}
