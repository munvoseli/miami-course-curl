#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define TOKEN_BYTES 41
#define TOKEN_LEN 40
#define VERBOSE_CURL

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

#include "first-callback.c"
#include "second-callback.c"
#include "third-callback.c"

int main( int argc, char** argv )
{
	char* a_cookie_sec [3];
	char* cookies_second = malloc(10);
	char* cookies_csv = malloc(10);
	sprintf( cookies_second, "Cookie: " );
	sprintf( cookies_csv, "Cookie: " );
	char* formtoken_second = malloc(TOKEN_BYTES);
	char* formtoken_csv = malloc(TOKEN_BYTES);

	curl_global_init( CURL_GLOBAL_ALL );
	do_first_request( &a_cookie_sec, &formtoken_second );
	printf("\n\nHHHHHHHHHHHHHHHH\n\n");
	//printf("%s\n%s\n", cookies, formtoken);
	do_second_request( cookies_second, formtoken_second, &cookies_csv );
	printf("\n\nIIIIIIIIIIIIIIII\n\n");
	printf("%s\n\nToken page: %s\n\n%s\n\n", cookies_second, formtoken_second, cookies_csv);
	put_bb( cookies_second, &cookies_csv ); // probably should've designed a better system earlier
	do_third_request( cookies_csv );
	
	// cleanup
	free(cookies_second);
	free(cookies_csv);
	free(formtoken_second);
	free(formtoken_csv);
	curl_global_cleanup();
	return 0;
}
