

unsigned int get_column_count( )
{
	FILE* fp = fopen( "out.csv", "r" );
	char in_quote = 0;
	char escaped = 0;
	unsigned int commas = 0;
	int c;
	while (1)
	{
		c = fgetc(fp);
		switch (c)
		{
		case '\n': if (in_quote) escaped = 0; break;
		case EOF: goto end;
		case '"': if (!escaped) in_quote ^= 1; escaped = 0; break;
		case ',': if (!in_quote) ++commas; escaped = 0; break;
		case '\\': escaped = 1; break;
		default: escaped = 0;
		}
	}
end:
	fclose(fp);
	return commas + 1;
}

void push_number( unsigned int** parr,
		  unsigned int* plen,
		  unsigned int num )
{
	++*plen;
	*parr = reallocarray( *parr, *plen, sizeof(unsigned int) );
	(*parr)[*plen - 1] = num;
}

unsigned int* get_csv_bounds( )
{
	unsigned int c_bound = 0;
	unsigned int* a_bound = NULL;
	FILE* fp = fopen( "out.csv", "r" );
	char in_quote = 0;
	char escaped = 0;
	unsigned int commas = 0;
	int c;
	int i = 0;
	for (int ci = 0; ; ++ci)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			break;
		}
		else if (in_quote)
		{
			if (escaped)
				continue;
			if (c == '"')
			{
				in_quote = 0;
				push_number( &a_bound, &c_bound, ci );
				//printf("End: %6d\n", ci);
			}
			escaped = c == '\\';
		}
		else
		{
			if (c == '"')
			{
				push_number( &a_bound, &c_bound, ci + 1 );
				//printf("Begin: %6d      ", ci + 1);
				in_quote = 1;
			}
		}
	}
	for (int i = 0; i < c_bound; i += 2)
	{
		//printf("%d %d  ", a_bound[i], a_bound[i + 1]);
		unsigned int diff = a_bound[i + 1] - a_bound[i];
		if ( diff > 100 )
		{
			char* str = malloc(1 + diff);
			str[diff] = 0;
			fseek( fp, a_bound[i], SEEK_SET );
			fread( str, 1, diff, fp );
			printf( "%s", str );
			break;
		}
	}
	fclose(fp);
	printf("\n\n");
	return a_bound;
}

