
/*************************************************************************************************/

/*** Defines ***/

#define RC_OK             0
#define RC_BAD           -1
#define RC_INVALID_INPUT -2

#define MAC_ADDR_MAX_LEN 13

/*************************************************************************************************/

/*** Interface ***/

bool char_is_a_num(char c);
uint8_t char_to_int(char c);
uint8_t char_to_int_hex(char c);
uint32_t cstr_count_char(const char* str_in, const size_t str_in_len, const char c);
uint32_t cstr_count_words(const char* str_in, const size_t str_in_len);
void str_rm_char(char* str, const char c_remove, char* readed=NULL);
bool str_contains(const char* s1, const char* s2);
bool str_read_until_char(char* str, const char c, char* readed=NULL, const bool preserve=false);
bool str_read_until_word(char* str, const char* word, char* readed=NULL, const bool preserve=false);
size_t str_read_between_chars(char* str, const char c, char* readed, const size_t readed_len, size_t start_at=0, const bool preserve=true);
bool str_read_line(char* str, char* readed=NULL, const bool preserve=false);
void str_rm_left_zeros(char* str, char* readed=NULL);
int str_to_int(const char* str_in);
uint8_t get_json_param_string_val(char* json_str, char* key, char* value, const size_t value_len);
uint8_t safe_atoi_u16(const char* in_str, const uint8_t in_str_len, uint16_t* out_int);
uint8_t cstr_is_num(const char* str_in, const size_t str_in_len);
char* int_to_str(int int_in);
bool str_is_IP(const char* str);
int32_t cstr_get_substr_between(const char* str_input, const size_t str_input_len, 
    const char* substr1, const char* substr2, char* str_output, const size_t str_output_len);
void cstr_lower(char* cstr, const size_t cstr_max_length);
void cstr_upper(char* cstr, const size_t cstr_max_length);
int8_t is_valid_mac_address(const char* mac_address);
bool is_a_valid_string_version(const char* str_ver, const uint8_t str_ver_len);
int8_t cstr_is_hex(char* str_in, const size_t str_in_len);
int8_t cstr_split_into_words(const char* str_in, const size_t str_in_len, char* words[], 
    size_t* num_words, const size_t max_num_words, const size_t max_word_length);

// Convert string into unsigned 8 bytes value
int8_t cstr_string_to_u8(char* str_in, size_t str_in_len, uint8_t* value_out, uint8_t base);

// Convert string into unsigned 16 bytes value
int8_t cstr_string_to_u16(char* str_in, size_t str_in_len, uint16_t* value_out, uint8_t base);

// Convert string into unsigned 32 bytes value
int8_t cstr_string_to_u32(char* str_in, size_t str_in_len, uint32_t* value_out, uint8_t base);

// Convert string into unsigned 64 bytes value
int8_t cstr_string_to_u64(char* str_in, size_t str_in_len, uint64_t* value_out, uint8_t base);

// Convert string into signed 8 bytes value
int8_t cstr_string_to_i8(char* str_in, size_t str_in_len, int8_t* value_out, uint8_t base);

// Convert string into signed 16 bytes value
int8_t cstr_string_to_i16(char* str_in, size_t str_in_len, int16_t* value_out, uint8_t base);

// Convert string into signed 32 bytes value
int8_t cstr_string_to_i32(char* str_in, size_t str_in_len, int32_t* value_out, uint8_t base);

// Convert string into signed 64 bytes value
int8_t cstr_string_to_i64(char* str_in, size_t str_in_len, int64_t* value_out, uint8_t base);

/*************************************************************************************************/

/*** Implementation ***/

bool char_is_a_num(char c)
{
	return ((c >= '0') && (c <= '9'));
}

uint8_t char_to_int(char c)
{
	uint8_t n = 0;

	if ((c >= '0') && (c <= '9'))
		n = c - '0';
	else
		n = 255;

	return n;
}

uint8_t char_to_int_hex(char c)
{
	uint8_t n = 0;

	if ((c >= '0') && (c <= '9'))
		n = c - '0';
	else if ((c >= 'a') && (c <= 'f'))
		n = c - 'a' + 10;
	else if ((c >= 'A') && (c <= 'F'))
		n = c - 'A' + 10;
	else
		n = 255;

	return n;
}

// Count the number of a character inside a string
uint32_t cstr_count_char(const char* str_in, const size_t str_in_len, const char c)
{
    uint32_t n = 0;

    if(str_in_len == 0)
        return 0;

    for(size_t i = 0; i < str_in_len; i++)
    {
        if(str_in[i] == c)
            n = n + 1;
    }

    return n;
}

// Count the number of words inside a string
uint32_t cstr_count_words(const char* str_in, const size_t str_in_len)
{
    uint32_t n = 1;

    // Check if string is empty
    if(str_in[0] == '\0')
        return 0;

    // Check for character occurrences
    for(size_t i = 1; i < str_in_len; i++)
    {
        // Check if end of string detected
        if(str_in[i] == '\0')
            break;

        // Check if pattern "X Y", "X\rY" or "X\nY" does not meet
        if((str_in[i] != ' ') && (str_in[i] != '\r') && (str_in[i] != '\n'))
            continue;
        if((str_in[i-1] == ' ') || (str_in[i-1] == '\r') || (str_in[i-1] == '\n'))
            continue;
        if((str_in[i+1] == ' ') || (str_in[i+1] == '\r') || (str_in[i+1] == '\n'))
            continue;
        if(str_in[i+1] == '\0')
            continue;

        // Pattern detected, increase word count
        n = n + 1;
    }

    return n;
}

// Remove all specific char from a string (str: "1 2 3 4 5" -> remove ' ' -> str: "12345")
void str_rm_char(char* str, const char c_remove, char* readed)
{
	uint16_t a, b; // strlen(str) < 65536

	a = 0;
	if(readed == NULL)
	{
		for(b = 0; b <= strlen(str); b++)
		{
			if(str[b] != c_remove)
			{
				str[a] = str[b];
				a = a + 1;
			}
		}
		str[a] = '\0';
	}
	else
	{
		for(b = 0; b <= strlen(str); b++)
		{
			if(str[b] != c_remove)
			{
				readed[a] = str[b];
				a = a + 1;
			}
		}
		readed[a] = '\0';
	}
}

// Check if a string contains a subtring  (str: "asdf qwerty lkjh" -> remove " qwerty" -> str: "asdf lkjh")
bool str_contains(const char* s1, const char* s2)
{
	size_t length_s1 = strlen(s1);
	size_t length_s2 = strlen(s2);
	
	if((length_s1 == 0) || (length_s2 == 0))
		return false;
	if(length_s1 < length_s2)
		return false;
	
	char substr[length_s2+1];
	for (size_t i = 0; i < length_s1+1; i++)
	{
		if(i+length_s2 < length_s1+1)
		{
			memcpy(substr, &s1[i], length_s2);
			substr[length_s2] = '\0';
			if (strcmp(substr, s2) == 0)
				return true;
		}
		else
			break;
	}
	
	return false;
}

// Read a line of an array
bool str_read_line(char* str, char* readed, const bool preserve)
{
	uint16_t i;
	bool found = false;

	if(str_read_until_char(str, '\n', readed, preserve))
	{
		// Remove '\n' character in readed data
		readed[strlen(readed)-1] = '\0';
	}

	return found;
}

// Read array until a specific char. The input string (str) split his content into readed chars (readed) and keep not readed ones
bool str_read_until_char(char* str, const char c, char* readed, const bool preserve)
{
	const uint16_t readed_len = strlen(str);
	char char_read = '\0';
	uint16_t a, b, i;
	bool found = false;
	
	i = 0;
	while(i < strlen(str) + 1)
	{
		char_read = str[i];
		//printf("%c", char_read);

		if(i < readed_len)
		{
			if(readed != NULL)
				readed[i] = char_read;
		}

		i = i + 1;

		if(char_read == c)
		{
			if(readed != NULL)
				readed[i] = '\0';

			// Remove readed data from str
			if(!preserve)
			{
				a = 0; b = i;
				while(b < strlen(str))
				{
					str[a] = str[b];
					a = a + 1;
					b = b + 1;
				}
				str[a] = '\0';
			}

			found = true;
			break;
		}
	}

	if(readed != NULL)
		readed[readed_len] = '\0';

	return found;
}

// Read array until a specific word. The input string (str) split his content into readed chars (readed) and keep not readed ones
bool str_read_until_word(char* str, const char* word, char* readed, const bool preserve)
{
	const uint16_t readed_len = strlen(str);
	char* read_word = new char[strlen(word) + 1]();
	char char_read = '\0';
	uint16_t a, b, i;
	bool found = false;

	a = 0;	b = 0;	i = 0;
	memset(read_word, '\0', sizeof(strlen(word) + 1));
	while(i < strlen(str) + 1)
	{
		char_read = str[i];
		//printf("%c", char_read);

		if((readed_len > 0) && (a < readed_len - 1))
		{
			if(readed != NULL)
				readed[a] = char_read;
			a = a + 1;
		}

		if(char_read == word[b])
		{
			if(b < strlen(word))
			{
				read_word[b] = char_read;
				b = b + 1;
			}

			if(b == strlen(word))
			{
				read_word[b] = '\0';
				if(strcmp(read_word, word) == 0)
				{
					if(readed != NULL)
						readed[a] = '\0';

					// Remove readed data from str
					if(!preserve)
					{
						a = 0;
						i = i + 1;
						while(i < strlen(str))
						{
							str[a] = str[i];
							a = a + 1;
							i = i + 1;
						}
						str[a] = '\0';
					}

					found = true;
					break;
				}
			}
		}
		else
		{
			b = 0;
			memset(read_word, '\0', sizeof(strlen(word) + 1));
		}

		i = i + 1;
	}

	if(readed != NULL)
		readed[readed_len] = '\0';

	return found;
}

size_t str_read_between_chars(char* str, const char c, char* readed, const size_t readed_len, size_t start_at, const bool preserve)
{
	size_t str_len = strlen(str);
	size_t second_char_at = 0;
	bool first_char_reach = false;
	bool second_char_reach = false;
	size_t r = 0;
	char read_c;
	
	// If start position if out of string
	if(start_at >= str_len-1)
		return 0;
	
	// Go through the string and search for substring
	for(size_t i = start_at; i < str_len; i++)
	{
		read_c = str[i];
		if(read_c == c)
			first_char_reach = true;
		
		if(first_char_reach)
		{
			if(read_c == c)
			{
				second_char_reach = true;
				second_char_at = i + 1;
				break;
			}
			
			if(r < readed_len-1)
			{
				readed[r] = read_c;
				r = r + 1;
			}
		}
	}
	
	// Set end of string character in read
	readed[r] = '\0';
}

// Remove left '0's of a string (str: "000012gd00adw0" -> str: "12gd00adw0")
void str_rm_left_zeros(char* str, char* readed)
{
	uint16_t a, b; // strlen(str) < 65536
	bool zero_yet;

	if(str[0] == '0')
		zero_yet = true;
	else
		zero_yet = false;

	a = 0;
	if(readed == NULL)
	{
		for(b = 0; b < strlen(str); b++)
		{
			if(zero_yet)
			{
				if(str[b] != '0')
				{
					str[a] = str[b];
					a = a + 1;
					zero_yet = false;
				}
			}
			else
			{
				str[a] = str[b];
				a = a + 1;
			}
		}
		str[a] = '\0';
	}
	else
	{
		for(b = 0; b < strlen(str); b++)
		{
			if(zero_yet)
			{
				if(str[b] != '0')
				{
					readed[a] = str[b];
					a = a + 1;
					zero_yet = false;
				}
			}
			else
			{
				readed[a] = str[b];
				a = a + 1;
			}
		}
		readed[a] = '\0';
	}
}

// Convert int to string
char* int_to_str(int int_in)
{
	char* str_out = new char[sizeof(int)+1]();

	memset(str_out, '\0', sizeof(int)+1);
	sprintf(str_out, "%d", int_in);

	return str_out;
}

// Convert string to int
int str_to_int(const char* str_in)
{
	int int_out = 0;

	for(int i = 0; i < strlen(str_in); i++)
		int_out = int_out * 10 + (str_in[i] - '0');

	return int_out;
}

// Rudimentary Parser for json string parameter
// Example: { "data" : "1234" } -> Get 1234
// It doesn't support deep json structures (Unsupported: { "data": { subdata":1234 } })
// It doesn't verify that all json string is correct (Uncheck: { "data": "1234" as}}df-g})
uint8_t get_json_param_string_val(char* json_str, char* key, char* value, const size_t value_len)
{
    char* ptr;
    uint32_t pos;
    char key_delimited[32];

    snprintf(key_delimited, 32, "\"%s\"", key);
    ptr = strstr(json_str, key_delimited);
    if(!ptr)
        return false;
    pos = ptr - json_str;
    json_str = json_str + pos + strlen(key_delimited);

    ptr = strstr(ptr, ":");
    if(!ptr)
        return false;
    pos = ptr - json_str;
    json_str = json_str + pos  + strlen(":");

    ptr = strstr(ptr, "\"");
    if(!ptr)
        return false;
    pos = ptr - json_str;
    json_str = json_str + pos  + strlen("\"");

    ptr = strstr(ptr+1, "\"");
    if(!ptr)
        return false;

    ptr[0] = '\0';
    for(uint32_t i = 0; i < strlen(json_str); i++)
    {
        if(i >= value_len)
            break;
        value[i] = json_str[i];
    }

    return true;
}

// Safe conversion a string number into uint16_t element
// in_str_len - number of bytes of in_str[] without null terminated byte
// Return: 0 Error, 1 OK
uint8_t safe_atoi_u16(const char* in_str, const uint8_t in_str_len, uint16_t* out_int)
{
    size_t converted_num;
    size_t multiplicator;

    // Check if input str has less or more chars than expected int32_t range (1 to 3 chars)
    if((in_str_len < 1) || (in_str_len > 6))
        return 0;

    // Check if input str is not terminated
    if(in_str[in_str_len] != '\0')
        return 0;

    // Check if any of the character of the str is not a number
    for(uint8_t i = 0; i < in_str_len; i++)
    {
        if(in_str[i] < '0' || in_str[i] > '9')
            return 0;
    }

    // Create the int
    converted_num = 0;
    for(uint8_t i = 0; i < in_str_len; i++)
    {
        multiplicator = 1;
        for(uint8_t ii = in_str_len-1-i; ii > 0; ii--)
            multiplicator = multiplicator * 10;

        converted_num = converted_num + (multiplicator * (in_str[i] - '0'));
    }

    // Check if number is higher than max uint16_t val
    if(converted_num > 65535)
        return 0;

    // Get the converted number and return operation success
    *out_int = (uint16_t)converted_num;
    return 1;
}

// Check if a string is a valid decimal number
uint8_t cstr_is_num(const char* str_in, const size_t str_in_len)
{
    if(str_in_len == 0)
        return 0;

    for(size_t i = 0; i < str_in_len; i++)
    {
        if((str_in[i] < '0') || (str_in[i] > '9'))
            return 0;
    }

    return 1;
}

// Check if a string has a valid IP format
bool str_is_IP(const char* str)
{
	bool is_ip = false;
	uint8_t i;
	uint8_t num_dots = 0;
	int8_t dots_places[3] = { -1, -1, -1 };

	// If str has a max lenght equal to max IP lenght or less
	if(strlen(str) <= 15)
	{
		// Check if the format of the string is corresponding to an IP adderss
		for(i = 0; i < strlen(str); i++)
		{
			// If there is a '.'
			if(str[i] == '.')
			{
				if(dots_places[0] == -1)
					dots_places[0] = i;
				else if(dots_places[1] == -1)
					dots_places[1] = i;
				else if(dots_places[2] == -1)
					dots_places[2] = i;

				num_dots++;
			}
			// If there is a null end of string character
			else if(str[i] == '\0')
				break;
			// If there is a character that is not a '.', '\0' or a number
			else if((str[i] < '0') || (str[i] > '9'))
			{
				num_dots = 0;
				break;
			}
		}

		// If there is 3 points in the string
		if(num_dots == 3)
		{
			// Check if points '.' are in correct places (min one number between points)
			if((dots_places[0] > 0) && (dots_places[0] <= 3))
			{
				if((dots_places[1] > dots_places[0] + 1) && (dots_places[1] <= 7))
				{
					if((dots_places[2] > dots_places[1] + 1) && (dots_places[1] <= 11))
						is_ip = true;
				}
			}
		}
	}

	return is_ip;
}

// Get a substring between two substrings of a provided string
// Results:
//   >0 - Found at this start index position of input string
//    0 - Not found
//   -1 - Some of the provided arguments is invalid
//   -2 - Found, but the provided output string length is not enough for store it
int32_t cstr_get_substr_between(const char* str_input, const size_t str_input_len, 
    const char* substr1, const char* substr2, char* str_output, const size_t str_output_len)
{
    char* p1 = NULL;
    char* p2 = NULL;
    size_t pos1 = 0, pos2 = 0;
    size_t i = 0, ii = 0;

    // Check for valid arguments
    if(str_input == NULL)
        return 0;
    if(str_input_len == 0)
        return 0;
    if(substr1 == NULL)
        return 0;
    if(strlen(substr1) == 0)
        return 0;
    if(substr2 == NULL)
        return 0;
    if(strlen(substr2) == 0)
        return 0;

    // Check for substrings and get index positions
    p1 = strstr((char*)str_input, substr1);
    if(p1 == NULL)
        return 0;
    p1 = p1 + strlen(substr1);
    pos1 = (p1 - str_input) - 1;
    p2 = strstr(p1, substr2);
    if(p2 == NULL)
        return 0;
    pos2 = pos1 + (p2 - p1) + 1;

    // Check if output string length is enough for the located substring
    if(str_output_len <= pos2-pos1)
        return -2;

    // Copy substring value into output string
    i = 0;
    ii = pos1+1;
    while(ii < pos2)
    {
        if(i >= str_output_len-1)
            break;

        str_output[i] = str_input[ii];
        i = i + 1;
        ii = ii + 1;
    }
    str_output[i] = '\0';

    if(str_output[0] != '\0')
        pos1 = pos1+1;

    return pos1;
}

void cstr_lower(char* cstr, const size_t cstr_max_length)
{
    char c;

    for(size_t i = 0; i < cstr_max_length; i++)
    {
        c = cstr[i];

        // Break if end of string charater has arrive
        if(c == '\0')
            break;

        // Convert extected basic ascii upper to lower
        switch(c)
        {
            case 'A': cstr[i] = 'a'; break;
            case 'B': cstr[i] = 'b'; break;
            case 'C': cstr[i] = 'c'; break;
            case 'D': cstr[i] = 'd'; break;
            case 'E': cstr[i] = 'e'; break;
            case 'F': cstr[i] = 'f'; break;
            case 'G': cstr[i] = 'g'; break;
            case 'H': cstr[i] = 'h'; break;
            case 'I': cstr[i] = 'i'; break;
            case 'J': cstr[i] = 'j'; break;
            case 'K': cstr[i] = 'k'; break;
            case 'L': cstr[i] = 'l'; break;
            case 'M': cstr[i] = 'm'; break;
            case 'N': cstr[i] = 'n'; break;
            case 'O': cstr[i] = 'o'; break;
            case 'P': cstr[i] = 'p'; break;
            case 'Q': cstr[i] = 'q'; break;
            case 'R': cstr[i] = 'r'; break;
            case 'S': cstr[i] = 's'; break;
            case 'T': cstr[i] = 't'; break;
            case 'U': cstr[i] = 'u'; break;
            case 'V': cstr[i] = 'v'; break;
            case 'W': cstr[i] = 'w'; break;
            case 'X': cstr[i] = 'x'; break;
            case 'Y': cstr[i] = 'y'; break;
            case 'Z': cstr[i] = 'z'; break;
            default: break;
        }
    }
}

void cstr_upper(char* cstr, const size_t cstr_max_length)
{
    char c;

    for(size_t i = 0; i < cstr_max_length; i++)
    {
        c = cstr[i];

        // Break if end of string charater has arrive
        if(c == '\0')
            break;

        // Convert extected basic ascii upper to lower
        switch(c)
        {
            case 'a': cstr[i] = 'A'; break;
            case 'b': cstr[i] = 'B'; break;
            case 'c': cstr[i] = 'C'; break;
            case 'd': cstr[i] = 'D'; break;
            case 'e': cstr[i] = 'E'; break;
            case 'f': cstr[i] = 'F'; break;
            case 'g': cstr[i] = 'G'; break;
            case 'h': cstr[i] = 'H'; break;
            case 'i': cstr[i] = 'I'; break;
            case 'j': cstr[i] = 'J'; break;
            case 'k': cstr[i] = 'K'; break;
            case 'l': cstr[i] = 'L'; break;
            case 'm': cstr[i] = 'M'; break;
            case 'n': cstr[i] = 'N'; break;
            case 'o': cstr[i] = 'O'; break;
            case 'p': cstr[i] = 'P'; break;
            case 'q': cstr[i] = 'Q'; break;
            case 'r': cstr[i] = 'R'; break;
            case 's': cstr[i] = 'S'; break;
            case 't': cstr[i] = 'T'; break;
            case 'u': cstr[i] = 'U'; break;
            case 'v': cstr[i] = 'V'; break;
            case 'w': cstr[i] = 'W'; break;
            case 'x': cstr[i] = 'X'; break;
            case 'y': cstr[i] = 'Y'; break;
            case 'z': cstr[i] = 'Z'; break;
            default: break;
        }
    }
}

int8_t is_valid_mac_address(const char* mac_address)
{
    char c;

    // Check if has the expected length of a MAC Address
    if(strlen(mac_address) != MAC_ADDR_MAX_LEN-1)
        return RC_BAD;

    // Check if has valid characters of a MAC Address (0-9/a-f)
    for(uint8_t i = 0; i < MAC_ADDR_MAX_LEN; i++)
    {
        c = mac_address[i];
        if((c < '0') || (c > '9'))
        {
            if((c != 'a') && (c != 'b') && (c != 'c') && (c != 'd') && (c != 'e') && (c != 'f'))
            {
                if((c != 'A') && (c != 'B') && (c != 'C') && (c != 'D') && (c != 'E') && (c != 'F'))
                    return RC_BAD;
            }
        }
    }

    return RC_OK;
}

bool is_a_valid_string_version(const char* str_ver, const uint8_t str_ver_len)
{
	uint8_t num_dots = 0;

	// Check if string version has an invalid number of characters
	// Expected number of characters in range: "X.Y.Z" to "XXX.YYY.ZZZ"
	if((str_ver_len < 5) || (str_ver_len > 11))
		return false;

	// Check if version string has 3 dots and expected characters
	for(uint8_t i = 0; i < str_ver_len; i++)
	{
		// Check for invalid character
		if((str_ver[i] < '0' || str_ver[i] > '9') && (str_ver[i] != '.'))
			return false;

		// Count dots
		if(str_ver[i] == '.')
			num_dots = num_dots + 1;
	}

	// Check for unexpected number of dots
	if(num_dots != 2)
		return false;

	return true;
}

// Check if string is a valid hexadecimal value
int8_t cstr_is_hex(char* str_in, const size_t str_in_len)
{
    for(size_t i = 0; i < str_in_len; i++)
    {
        // Check ascii table positions out of 0-9/a-f/A-F character ranges
        if(str_in[i] < '0')
            return RC_BAD;
        if(str_in[i] > '9')
        {
            if(str_in[i] < 'A')
                return RC_BAD;
            if(str_in[i] > 'F')
            {
                if(str_in[i] < 'a')
                    return RC_BAD;
                if(str_in[i] > 'f')
                    return RC_BAD;
            }
        }
    }

    return RC_OK;
}

// Convert string into unsigned 8 bytes value
int8_t cstr_string_to_u8(char* str_in, size_t str_in_len, uint8_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    uint64_t tmp = (uint64_t)(*value_out);

    rc = cstr_string_to_u64(str_in, str_in_len, &tmp, base);
    *value_out = (uint8_t)(tmp);

    return rc;
}

// Convert string into unsigned 16 bytes value
int8_t cstr_string_to_u16(char* str_in, size_t str_in_len, uint16_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    uint64_t tmp = (uint64_t)(*value_out);

    rc = cstr_string_to_u64(str_in, str_in_len, &tmp, base);
    *value_out = (uint16_t)(tmp);

    return rc;
}

// Convert string into unsigned 32 bytes value
int8_t cstr_string_to_u32(char* str_in, size_t str_in_len, uint32_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    uint64_t tmp = (uint64_t)(*value_out);

    rc = cstr_string_to_u64(str_in, str_in_len, &tmp, base);
    *value_out = (uint32_t)(tmp);

    return rc;
}

// Convert string into unsigned 64 bytes value
int8_t cstr_string_to_u64(char* str_in, size_t str_in_len, uint64_t* value_out, uint8_t base)
{
    char* ptr = str_in;
    uint8_t digit;

    *value_out = 0;

    // Check and ignore + symbol
    if(ptr[0] == '+')
    {
        if(str_in_len < 2)
            return RC_INVALID_INPUT;
        ptr = ptr + 1;
        str_in_len = str_in_len - 1;
    }

    // Check for hexadecimal "0x" bytes and go through it
    if(base == 16)
    {
        if((ptr[1] == 'x') || (ptr[1] == 'X'))
        {
            if(str_in_len < 3)
                return RC_INVALID_INPUT;
            ptr = ptr + 2;
            str_in_len = str_in_len - 2;
        }
    }

    // Convert each byte of the string
    for(uint16_t i = 0; i < str_in_len; i++)
    {
        if(base == 10)
        {
            if(ptr[i] >= '0' && ptr[i] <= '9')
                digit = ptr[i] - '0';
            else
                return RC_INVALID_INPUT;
        }
        else if(base == 16)
        {
            if(ptr[i] >= '0' && ptr[i] <= '9')
                digit = ptr[i] - '0';
            else if (base == 16 && ptr[i] >= 'a' && ptr[i] <= 'f')
                digit = ptr[i] - 'a' + 10;
            else if (base == 16 && ptr[i] >= 'A' && ptr[i] <= 'F')
                digit = ptr[i] - 'A' + 10;
            else
                return RC_INVALID_INPUT;
        }
        else
            return RC_INVALID_INPUT;

        *value_out = ((*value_out)*base) + digit;
    }

    return RC_OK;
}

// Convert string into signed 8 bytes value
int8_t cstr_string_to_i8(char* str_in, size_t str_in_len, int8_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    int64_t tmp = (int64_t)(*value_out);

    rc = cstr_string_to_i64(str_in, str_in_len, &tmp, base);
    *value_out = (int8_t)(tmp);

    return rc;
}

// Convert string into signed 16 bytes value
int8_t cstr_string_to_i16(char* str_in, size_t str_in_len, int16_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    int64_t tmp = (int64_t)(*value_out);

    rc = cstr_string_to_i64(str_in, str_in_len, &tmp, base);
    *value_out = (int16_t)(tmp);

    return rc;
}

// Convert string into signed 32 bytes value
int8_t cstr_string_to_i32(char* str_in, size_t str_in_len, int32_t* value_out, uint8_t base)
{
    uint8_t rc = 0;
    int64_t tmp = (int64_t)(*value_out);

    rc = cstr_string_to_i64(str_in, str_in_len, &tmp, base);
    *value_out = (int32_t)(tmp);

    return rc;
}

// Convert string into signed 64 bytes value
int8_t cstr_string_to_i64(char* str_in, size_t str_in_len, int64_t* value_out, uint8_t base)
{
    char* ptr = str_in;
    uint8_t digit;
    uint8_t negative_number = 0;

    *value_out = 0;

    // Check for + and - symbols
    if((ptr[0] == '+') || (ptr[0] == '-'))
    {
        if(str_in_len < 2)
            return RC_INVALID_INPUT;

        if(ptr[0] == '-')
            negative_number = 1;

        ptr = ptr + 1;
        str_in_len = str_in_len - 1;
    }

    // Check for hexadecimal "0x" bytes and go through it
    if(base == 16)
    {
        if((ptr[1] == 'x') || (ptr[1] == 'X'))
        {
            if(str_in_len < 3)
                return RC_INVALID_INPUT;
            ptr = ptr + 2;
            str_in_len = str_in_len - 2;
        }
    }

    // Convert each byte of the string
    for(uint16_t i = 0; i < str_in_len; i++)
    {
        if(base == 10)
        {
            if(ptr[i] >= '0' && ptr[i] <= '9')
                digit = ptr[i] - '0';
            else
                return RC_INVALID_INPUT;
        }
        else if(base == 16)
        {
            if(ptr[i] >= '0' && ptr[i] <= '9')
                digit = ptr[i] - '0';
            else if (base == 16 && ptr[i] >= 'a' && ptr[i] <= 'f')
                digit = ptr[i] - 'a' + 10;
            else if (base == 16 && ptr[i] >= 'A' && ptr[i] <= 'F')
                digit = ptr[i] - 'A' + 10;
            else
                return RC_INVALID_INPUT;
        }
        else
            return RC_INVALID_INPUT;

        *value_out = ((*value_out)*base) + digit;
    }

    // Turn into negative if needs
    if(negative_number)
        *value_out = (*value_out)*(-1);

    return RC_OK;
}

// Split a string into array of strings for each word
int8_t cstr_split_into_words(const char* str_in, const size_t str_in_len, char* words[], 
    size_t* num_words, const size_t max_num_words, const size_t max_word_length)
{
    size_t i = 0;
    size_t ii = 0;
    size_t iii = 0;

    // Check if provided input string length is 0
    if(str_in_len == 0)
        return RC_INVALID_INPUT;

    while(i < str_in_len)
    {
        // Space character detected, close actual word and increase index to next one
        if(str_in[i] == ' ')
        {
            words[iii][ii] = '\0';
            i = i + 1;
            ii = 0;

            // If reached max number of words
            iii = iii + 1;
            if(iii >= max_num_words)
            {
                *num_words = iii;
                return RC_BAD;
            }

            continue;
        }

        // Store actual character in actual word
        words[iii][ii] = str_in[i];
        
        // If end of string reach, close actual word
        i = i + 1;
        if(i >= str_in_len)
            words[iii][ii] = '\0';

        // If reached word max length
        ii = ii + 1;
        if(ii >= max_word_length)
        {
            words[iii][max_word_length-1] = '\0';
            *num_words = iii;
            return RC_BAD;
        }
    }

    // Set detected number of words
    *num_words = iii;

    return RC_OK;
}

/*************************************************************************************************/

/*** Usage ***/

char str[256] = "000123-400\n0056-189-a11-qw0e-ty10";
char read[256] = "";

/*****************************************/

// Read Until Word
/*printf("Init:\n%s\n", str);
str_read_until_word(str, "-asd");
printf("\nRemain:\n%s\n", str);*/

/*printf("Init:\n%s\n", str);
str_read_until_word(str, "-asd", read);
printf("\nRead:\n%s\n", read);
printf("\nRemain:\n%s\n", str);*/

/*****************************************/

// Read Until Char
/*printf("Init:\n%s\n", str);
str_read_until_char(str, '9');
printf("\nRemain:\n%s\n", str);*/

/*printf("Init:\n%s\n", str);
str_read_until_char(str, '9', read);
printf("\nRead:\n%s\n", read);
printf("\nRemain:\n%s\n", str);*/

/*****************************************/

// Read Line
/*printf("Init:\n%s\n", str);
str_read_line(str, read, true);
printf("\nRead:\n%s\n", read);
printf("\nRemain:\n%s\n", str);*/

/*****************************************/

// String contains subtring
/*printf("Init:\n%s\n", str);
if(str_contains(str, "ale"))
	printf("\nContained\n");
else
	printf("\nNot contained\n");*/

/*****************************************/

// String Remove chars
/*printf("Init:\n%s\n", str);
str_rm_char(str, '1', read);
printf("\nRead:\n%s\n", read);
printf("\nRemain:\n%s\n", str);*/

/*****************************************/

// Remove left Zeros
/*printf("Init:\n%s\n", str);
str_rm_left_zeros(str);
printf("\nRead:\n%s\n", read);
printf("\nRemain:\n%s\n", str);*/

/*****************************************/
