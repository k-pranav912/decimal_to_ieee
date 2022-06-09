#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Node {
	int data;
	struct Node* next;
};

void clear_ll(struct Node* head) {
	struct Node* curr = head;
	struct Node* prev = 0;

	while (curr != 0) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	return;
}

void print_arr(int* arr, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d", arr[i]);
	}
	return;
}

void print_ll(struct Node* head) {
	if (head == 0) {
		//printf("EMPTY\n");
		return;
	}

	struct Node* curr = head;

	while (curr != 0) {
		if (curr->next == 0) {
			printf("%d", curr->data);
		} else {
			printf("%d", curr->data);
		}
		curr = curr->next;
	}

	return;

}

int length_ll(struct Node* head) {
	struct Node* curr = head;
	int count = 0;

	while (curr != 0) {
		curr = curr->next;
		count++;
	}

	return count;
}

struct Node* enqueue(int val, struct Node* head) {
	if (head == 0) {
		struct Node* new_node = malloc(sizeof(struct Node));
		new_node->data = val;
		new_node->next = 0;
		return new_node;
	}

	struct Node* curr = head;
	struct Node* prev = 0;

	while (curr != 0) {
		prev = curr;
		curr = curr->next;
	}

	struct Node* new_node = malloc(sizeof(struct Node));
	new_node->data = val;
	new_node->next = 0;
	prev->next = new_node;

	return head;

}

struct Node* push(int val, struct Node* head) {
	struct Node* new_node = malloc(sizeof(struct Node));
	new_node->data = val;
	new_node->next = head;
	return new_node;

	
}

struct Node* pop(struct Node* head) {
	if (head == 0) {return head;}
	struct Node* curr = head->next;
	free(head);
	return curr;
}

struct Node* bin_int(int num) {
	struct Node* head = 0;
	int tmp;

	while (num != 0) {

		tmp = num % 2;
		head = push(tmp, head);
		num = num / 2;
	}


	return head;
}

struct Node* bin_frac(double num, int digit) {
	double whole;
	struct Node* head = 0;

	for (int i = 0; i < digit; i++) {
		num = num * 2;
		num = modf(num, &whole);
		head = enqueue(whole, head);
	}

	return head;
	
}

struct Node* zero_whole(struct Node* head, int *x) {
	int count = 1;

	while (head->data != 1) {
		head = pop(head);
		count++;	
	}
	
	*x = (-1*count);
	return head;
}

void add_one(int* bin, int size, int *cf) {
	int carry = 1;

	for (int i =  (size - 1); i >= 0; i--) {
		bin[i] += carry;
		if (bin[i] == 2) {
			bin[i] = 0;
			carry = 1;		
		} else {
			carry = 0;
		}
	}

	*cf = carry;
	return;
}

int* unsigned_binary(int num, int digit) {
	int* bin = malloc(digit * sizeof(int));
	for (int i = 0; i < digit; i++) {
		bin[i] = 0;
	}

	int index = digit - 1;

	while (num != 0) {
		bin[index] = num % 2;
		index--;
		num = num /2;	
	}

	//print_arr(bin, digit);
	//free(bin);
	return bin;
}

int* canonical_bin(double num, int digit, int *E) {

	double whole = 0;
	double frac = 0;

	frac = modf(num, &whole);

	struct Node* whole_head = bin_int(whole);
	int frac_limit = digit - (length_ll(whole_head) - 1);
	struct Node* frac_head = bin_frac(frac, frac_limit);

	int* bin = malloc((digit + 1) * sizeof(int));
	for(int i = 0; i < digit+1; i++) {
		bin[i] = 0;
	}
	int index = 0;
	int exp = 0;

	if (whole_head != 0) {
		
		exp = length_ll(whole_head) - 1;

		for (struct Node* curr = whole_head; curr != 0; curr=curr->next) {
			if (index > digit) {
				break;
			}
			bin[index] = curr->data;
			index++;
		}
		
		for (struct Node* curr = frac_head; curr != 0; curr=curr->next) {
			if (index > digit) {
				break;
			}
			bin[index] = curr->data;
			index++;
		}

	} else {
		frac_head = zero_whole(frac_head, &exp);
	
		for (struct Node* curr = frac_head; curr != 0; curr=curr->next) {
			if (index > digit) {
				break;
			}
			bin[index] = curr->data;
			index++;
		}
	}

/*
	printf("%d.", bin[0]);
	
	for(int i = 1; i < digit+1; i++) {
		printf("%d", bin[i]);
	}
	printf(" %d\n", exp);
*/
	
	*E = exp;

	clear_ll(whole_head);
	clear_ll(frac_head);
	//free(bin);

	return bin;	

}

int mid_compare(int* bin, int frac_bits) {

/*
	printf("cmp f_val: ");
	print_arr(bin, 52);
	printf("\n");
*/

	int* mid = malloc(52*sizeof(int));
	for (int i = 0; i < 52; i++) {
		mid[i] = 0;
	}
	
	for (int i = 0; i < frac_bits; i++) {
		mid[i] = bin[i];
	}
	
	//printf("frac_bits: %d\n", frac_bits);
	mid[frac_bits] = 1;

/*
	printf("mid: ");
	print_arr(mid, 52);
	printf("\n");
*/

	for (int i = 0; i < 52; i++) {
		if (bin[i] > mid[i]) {
			free(mid);
			return 1;
		} else if (bin[i] < mid[i]) {
			free(mid);
			return -1;
		}
	}

	free(mid);


	return 0;

}

void denormal_repr(double num, int bits, int exp_bits, int frac_bits, int sign) {
	int E_val = 0;

	int* f_val = canonical_bin(num, 51, &E_val);

	int* exponent = malloc(exp_bits * sizeof(int));

	for (int i = 0; i < exp_bits; i++) {
		exponent[i] = 0;
	}

	int carry = 0;
	int* upper = malloc(frac_bits*sizeof(int));
	int* lower = malloc(frac_bits*sizeof(int));
	
	for (int i = 0; i < frac_bits; i++) {
		upper[i] = f_val[i];
		lower[i] = f_val[i];
	}

	add_one(upper, frac_bits, &carry);

	int cmp = mid_compare(f_val, frac_bits);

	printf("%d", sign);
	print_arr(exponent, exp_bits);
	
	if (cmp >= 0) {
		print_arr(upper, frac_bits);
	} else {
		print_arr(lower, frac_bits);
	}
	
	printf("\n");

	free(f_val);
	free(exponent);
	free(upper);
	free(lower);

	return;	
	
}

void fp_repr(double num, int bits, int exp_bits, int frac_bits) {
	int sign = 0;
	if (num < 0) {
		sign = 1;
		num = (-1)*num;	
	}

	int bias = (1 << (exp_bits -1)) - 1;

	if (num < (1.0/(1 << (bias - 1)))) {
		denormal_repr(num, bits, exp_bits, frac_bits, sign);
		return;
	}

	int E_val = 0;

	int* bin = canonical_bin(num, 52, &E_val);

/*
	printf("bin: ");
	print_arr(bin, 53);
	printf("\n");
*/



	int exp = E_val + bias; 

	int* f_val = malloc(52 * sizeof(int));

	for (int i = 0; i < 52; i++) {
		f_val[i] = bin[i+1];
	}

/*
	printf("f_val: ");
	print_arr(f_val, 52);
	printf("\n");
*/

	int carry = 0; 
	int* upper = malloc(frac_bits*sizeof(int));
	int* lower = malloc(frac_bits*sizeof(int));
	
	for (int i = 0; i < frac_bits; i++) {
		upper[i] = f_val[i];
		lower[i] = f_val[i];
	}

	add_one(upper, frac_bits, &carry);

	int cmp = mid_compare(f_val, frac_bits);

	if (cmp == 0) {
		if (upper[frac_bits - 1] == 0) {
			cmp = 1;
		} else {
			cmp = -1;
		}
	}

	//printf("CMP: %d\n", cmp);

	if (cmp >= 0) {
		exp += carry;
	}

	int* exponent = unsigned_binary(exp, exp_bits);

	printf("%d", sign);
	print_arr(exponent, exp_bits);
	
	if (cmp >= 0) {
		print_arr(upper, frac_bits);
	} else {
		print_arr(lower, frac_bits);
	}
	
	printf("\n");

	free(bin);
	free(f_val);
	free(upper);
	free(lower);
	free(exponent);

	return;
	
}

int main(int argc, char* argv[argc+1]) {
	

	double num;
	int bits;
	int exp_bits;
	int frac_bits;

	FILE* fp = fopen(argv[1], "r");

	if (fp == 0) {
		printf("No file found.\n");
		return EXIT_SUCCESS;
	}


	while(fscanf(fp, "%lf %d %d %d\n", &num, &bits, &exp_bits, &frac_bits) != EOF) {
		fp_repr(num, bits, exp_bits, frac_bits);
	}

/*
	double num = -15.5;
	int bits = 8;
	int exp_bits = 4;
	int frac_bits = 3;

	fp_repr(num, bits, exp_bits, frac_bits);
*/


	return EXIT_SUCCESS;	
}
