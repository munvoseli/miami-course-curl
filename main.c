#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define TOKEN_BYTES 41
#define TOKEN_LEN 40
#define VERBOSE_CURL
#define printf(a...)

char* get_token( char* html )
{
	char* begin;
	char* end;
	begin = strstr( html, "_token" );
	if (begin == NULL)
		return NULL;
	begin = strstr( begin, "value" ) + 7;
	end = strstr( begin, "\"" );
	end[0] = 0;
	return begin;
}

char* cookie_array_to_cookies( char* a_cookie [3] )
{
	int length =
		strlen(a_cookie[0]) +
		strlen(a_cookie[1]) +
		strlen(a_cookie[2]) + 13;
	char* str = malloc(length + 1);
	str[length] = 0;
	sprintf(str, "Cookie: %s; %s; %s", a_cookie[0], a_cookie[1], a_cookie[2]);
	return str;
}

#include "first-callback.c"
#include "second-callback.c"
#include "third-callback.c"

int main( int argc, char** argv )
{
	char* a_cookie [3];
	char* cookies_second;
	char* cookies_csv;
	char* formtoken_second = malloc(TOKEN_BYTES);
	char* formtoken_csv = malloc(TOKEN_BYTES);

	curl_global_init( CURL_GLOBAL_ALL );
	
	do_first_request( a_cookie, &formtoken_second );
	cookies_second = cookie_array_to_cookies(a_cookie);
	
	printf("\n\nHHHHHHHHHHHHHHHH\n\n");
	
	do_second_request( cookies_second, formtoken_second, a_cookie );
	cookies_csv = cookie_array_to_cookies(a_cookie);
	
	printf("\n\nIIIIIIIIIIIIIIII\n\n");
	printf("%s\n\nToken page: %s\n\n%s\n\n", cookies_second, formtoken_second, cookies_csv);
	
	do_third_request( cookies_csv );
	
	// cleanup
	free(a_cookie[0]);
	free(a_cookie[1]);
	free(a_cookie[2]);
	free(cookies_second);
	free(cookies_csv);
	free(formtoken_second);
	free(formtoken_csv);
	curl_global_cleanup();
	return 0;
}
