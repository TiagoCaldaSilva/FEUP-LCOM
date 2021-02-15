#include "utils.h"

int proj_util_sys_inb(uint8_t port, uint8_t *value) {
    uint32_t temp;
    if(sys_inb(port, &temp) != OK) return 1;
    *value = (uint8_t)temp;
    return 0;
}

bool between(uint16_t value, uint16_t min, uint16_t max){
    return (value <= max && value >= min);
}

int max(int n1, int n2) { return (n1 > n2) ? n1 : n2; }
int min(int n1, int n2) { return (n1 < n2) ? n1 : n2; }

void decimal_to_hex(int num, uint8_t * result){
	int temp = 0, quotient; 
	int * content = (int *)malloc(sizeof(int) + (temp + 1));	    	 
	
	content[temp] = num % 16;	 //The rest of the division by 16 is the value that should appear in the final value
	quotient = num / 16;		//In each iteration, the value should be divided by 16
		    		  
	while (quotient >= 16){		 	     	   
		temp++;		
		content = (int *)realloc(content, sizeof(int) * (temp + 1));		   
		content[temp] = quotient % 16; 
		quotient /= 16; 	   
	}

	temp++;
	content = (int *)realloc(content, sizeof(int) * (temp + 1));		               		  
	content[temp] = quotient;
	for (; temp >= 0; temp--){ //The last thing to do is put the values in the result variable
		*result = *result << 4;
		if (content[temp] >= 10){		 
			switch(content[temp]){
				case 10 : *result |= 0xA; break; 		
				case 11 : *result |= 0xB; break;
				case 12 : *result |= 0xC; break;
				case 13 : *result |= 0xD; break;
				case 14 : *result |= 0xE; break;
				case 15 : *result |= 0xF; break;						 
			}	
		}else *result |= content[temp];
	}
}
