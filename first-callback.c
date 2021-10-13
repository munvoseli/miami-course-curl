size_t first_get_callback( void* vdatap, size_t size, size_t nmemb, void* userp )
{
	char* datap = (char*) vdatap;
	char* token = get_token( datap );
	if (token)
		strncpy( userp, token, TOKEN_LEN );
	return nmemb;
}

size_t set_cookies_callback( void* vdatap, size_t size, size_t nmemb, void* userp )
{
	char* datap = (char*) vdatap;
	char* line;
	char* endline;
	unsigned int length;
	char** a_cookie = (char**) userp;
	line = strstr( datap, "Set-Cookie: " );
	if (line == datap)
	{
		line += 12;
		endline = strstr( line, ";" ) + 2;
		length = endline - line;
		endline[0] = 0;
		int i = line[0] == 'X' ? 0 :
			line[0] == 'l' ? 1 :
			line[1] == 'b' ? 2 : 3;
		a_cookie[i] = malloc( length + 1 );
		a_cookie[i][length] = 0;
		sprintf( a_cookie[i], line );
		//printf("%s\n\n", line);
	}
	return nmemb;
}

void do_first_request( char* a_cookies [3], char** p_token )
{
	CURLcode res;
	CURL* handle;
	char errbuf [CURL_ERROR_SIZE];
	
	handle = curl_easy_init();
	if (!handle)
		printf("H\n");

	// Do the first call (get) in order to set the cookies
	curl_easy_setopt( handle, CURLOPT_URL, "https://www.apps.miamioh.edu/courselist/" );
	curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, first_get_callback );
	curl_easy_setopt( handle, CURLOPT_WRITEDATA, *p_token );
	curl_easy_setopt( handle, CURLOPT_ERRORBUFFER, errbuf );
	curl_easy_setopt( handle, CURLOPT_HEADERFUNCTION, set_cookies_callback );
	curl_easy_setopt( handle, CURLOPT_HEADERDATA, a_cookies );
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) printf("error %d\n%s\n", res, errbuf);

	curl_easy_cleanup(handle);
}
