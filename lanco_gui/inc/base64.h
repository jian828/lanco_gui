#ifndef __BASE64_H_
#define __BASE64_H_

void init_base64_mode(unsigned char mode);
int isbase64(unsigned char ch);
int base64_encode (const unsigned char *read, unsigned short inlen, unsigned char *out, unsigned short outlen);
int base64_decode (const unsigned char *read, unsigned short inlen, unsigned char *out, unsigned short *outlen);

#endif
