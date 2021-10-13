size_t second_post_callback( void* vdatap, size_t size, size_t nmemb, void* userp )
{
	char* datap = (char*) vdatap;
	FILE* outfile = fopen( "out.html", "a" );
	fprintf(outfile, "%s\n", datap);
	fclose(outfile);
	return nmemb;
}

void do_second_request( const char* cookies,
			const char* token, char** a_cookie )
{
	CURLcode res;
	CURL* handle;
	char errbuf [CURL_ERROR_SIZE];
	
	handle = curl_easy_init();
	if (!handle)
		printf("H\n");
	
	// Do the second call (post)
	curl_easy_setopt( handle, CURLOPT_URL, "https://www.apps.miamioh.edu/courselist/" );
	struct curl_slist* chunk = NULL;
	chunk = curl_slist_append( chunk, cookies );
	#ifdef VERBOSE_CURL
	curl_easy_setopt( handle, CURLOPT_VERBOSE, 1L);
	#endif
	curl_easy_setopt( handle, CURLOPT_ERRORBUFFER, errbuf );
	curl_easy_setopt( handle, CURLOPT_HTTPHEADER, chunk );
	curl_easy_setopt( handle, CURLOPT_WRITEFUNCTION, second_post_callback );
	curl_easy_setopt( handle, CURLOPT_HEADERFUNCTION, set_cookies_callback );
	curl_easy_setopt( handle, CURLOPT_HEADERDATA, a_cookie );
	char postfields [512];
	sprintf( postfields, "_token=%s"
		 "&term=202220&campusFilter%5B%5D=O"
		 "&subject%5B%5D=CSE"
		 "&courseNumber=271"
		 "&openWaitlist=&crnNumber="
		 "&level=&courseTitle=&instructor=&instructorUid="
		 "&creditHours=&startEndTime%5B%5D="
		 "&startEndTime%5B%5D=&courseSearch=", token );
	curl_easy_setopt( handle, CURLOPT_POSTFIELDS, postfields);
	FILE* outfile = fopen( "out.html", "w" );
	fclose(outfile);
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) printf("error %d\n%s\n", res, errbuf);
	curl_easy_cleanup(handle);
	printf("h");
}
