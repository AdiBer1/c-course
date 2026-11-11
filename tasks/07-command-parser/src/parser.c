#include "parser.h"
#include <string.h>
#include <stddef.h>

/* TODO: implement both functions. Small static helper functions are welcome. */

parse_status_t parse_command(const char *line, command_t *out)
{
    (void)line;
    (void)out;
        if (line==NULL  || out == NULL) {
        return PARSE_ERR_NULL;
    }
   size_t len = strlen(line);
int had_newline = 0;
if (len > 0 && line[len - 1] == '\n'){
len--;
had_newline = 1;}
if (had_newline == 1 && len > 0 && line[len - 1] == '\r') 
len--;
if (len ==0)
   return PARSE_ERR_EMPTY;
  
size_t i=0;
while(i<len){
char c=line[i];
if(c=='=')
break;
if(i==0&&(c < 'A' || c > 'Z'))
return PARSE_ERR_BAD_KEY;
if((c < 'A' || c > 'Z')&&(c<'0'||c>'9')&&c!='_')
return PARSE_ERR_BAD_KEY;
i++;}
if(i==len)
return PARSE_ERR_NO_EQUALS;
if(i==0)
return PARSE_ERR_BAD_KEY;
if(i>PARSE_KEY_MAX)
return PARSE_ERR_KEY_TOO_LONG;
    size_t j = i + 1;
    if (j < len && line[j] == '-') {
        j++;
    }
    if (j == len) {
        return PARSE_ERR_BAD_VALUE;
    }

    size_t k = j;
    while (k < len) {
        if (line[k] < '0' || line[k] > '9') {
            return PARSE_ERR_BAD_VALUE;
        }
        k++;
    }
     int negative = (line[i + 1] == '-');

    int32_t value = 0;
    k = j;
    while (k < len) {
        int32_t digit = line[k] - '0';
        value = value * 10 + digit;
        if (value > PARSE_VALUE_MAX) {
            return PARSE_ERR_RANGE;
        }
        k++;
    }

    if (negative) {
        value = -value;
    }

    size_t m = 0;
    while (m < i) {
        out->key[m] = line[m];
        m++;
    }
    out->key[i] = '\0';
    out->value = value;

    return PARSE_OK;
}
const char *parse_status_str(parse_status_t status)
{
    (void)status;
       switch (status) {
    case PARSE_OK:
        return "PARSE_OK";
         case PARSE_ERR_EMPTY:
        return "PARSE_ERR_EMPTY";
         case PARSE_ERR_NO_EQUALS:
        return "PARSE_ERR_NO_EQUALS";
         case PARSE_ERR_BAD_KEY:
           return "PARSE_ERR_BAD_KEY";
            case PARSE_ERR_KEY_TOO_LONG:
        return "PARSE_ERR_KEY_TOO_LONG";
         case PARSE_ERR_BAD_VALUE:
        return "PARSE_ERR_BAD_VALUE";
         case PARSE_ERR_RANGE:
        return "PARSE_ERR_RANGE";
        case PARSE_ERR_NULL:
        return "PARSE_ERR_NULL";

    default:
        return "UNKNOWN";
    }
}
