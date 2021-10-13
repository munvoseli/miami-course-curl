// The callback for getting the CSV

size_t third_post_callback( void* vdatap, size_t size, size_t nmemb, void* userp )
{
	char* datap = (char*) vdatap;
	FILE* outfile = fopen( "out.csv", "a" );
	fprintf(outfile, "%s\n", datap);
	fclose(outfile);
	char* token = get_token( datap );
	if (token)
		strncpy( userp, token, TOKEN_LEN );
	return nmemb;
}


char* get_html_as_str()
{
	char* html;
	
	FILE* fp = fopen( "out.html", "r" );
        long begin, end;
	begin = ftell( fp );
	fseek( fp, 0L, SEEK_END );
	end   = ftell( fp );
	fseek( fp, 0L, SEEK_SET );
	int length = end - begin;
	html = malloc(length + 1);
	fread( html, 1, length, fp );
	html[length] = 0;
	return html;
}

char* get_postfields( ) // from out.html
{
	char* html = get_html_as_str();
	char* token, *term, *results;
	char* begin, *end;
	unsigned int length;
	// token=&term=&resultsTermCrns=0
	unsigned int total_length = 30;
	begin = strstr( html, "id=\"exportForm\"" ); // to form
	
	begin = strstr( begin, "value" ) + 7;
	end = strstr( begin, "\"" );
	length = end - begin;
	token = malloc( length + 1 );
	strncpy( token, begin, length );
	token[length] = 0;
	total_length += length;

	begin = strstr( end, "value" ) + 7;
	end = strstr( begin, "\"" );
	length = end - begin;
	term = malloc( length + 1 );
	strncpy( term, begin, length );
	term[length] = 0;
	total_length += length;

	begin = strstr( end, "value" ) + 7;
	end = strstr( begin, "\"" );
	length = end - begin;
	results = malloc( length + 1 );
	strncpy( results, begin, length );
	results[length] = 0;
	total_length += length;

	char* endstr = calloc(total_length, 1);
	sprintf(endstr, "token=%s&term=%s&resultsTermCrns=%s", token, term, results);
	
	free(html);
	
	return endstr;
}

size_t do_third_request( char* cookies )
{
	CURLcode res;
	CURL* handle;
	char errbuf [CURL_ERROR_SIZE];
	
	handle = curl_easy_init();
	if (!handle)
		printf("H\n");
	
	// Do the third call (post)
	curl_easy_setopt( handle, CURLOPT_URL, "https://www.apps.miamioh.edu/courselist/exportToCsv" );
	#ifdef VERBOSE_CURL
	curl_easy_setopt( handle, CURLOPT_VERBOSE, 1L);
	#endif
	curl_easy_setopt( handle, CURLOPT_ERRORBUFFER, errbuf );
	struct curl_slist* chunk = NULL;
	chunk = curl_slist_append( chunk, cookies );
	curl_easy_setopt( handle, CURLOPT_HTTPHEADER, chunk );
	curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, third_post_callback );
	char* postfields = get_postfields();
	printf("%s\n\n", postfields);
	curl_easy_setopt( handle, CURLOPT_POSTFIELDS, postfields);
	FILE* outfile = fopen( "out.csv", "w" );
	fclose(outfile);
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) printf("error %d\n%s\n", res, errbuf);
	curl_easy_cleanup(handle);
	free(postfields);
	return 0;
}

