#include<stdio.h>
#include<string.h>
#include<alloca.h>

/*几个排序算法的测试*/

const char ori_data[] = "637152827152387351253728142763";
int data_len = sizeof(ori_data) - 1;

typedef struct _data_pos {
	int data;
	int pos;
} data_pos;

void print_buff(data_pos *, int);
void print_buff_ij(data_pos *, int, int);
void my_qsort(data_pos *, int, int);
void my_msort(data_pos *, int, int);
void swap(data_pos *, int, int);
int lib_qsort_cmp(void * a, void * b);

data_pos *g_buff = NULL;

int main() {

	data_pos buff[sizeof(ori_data) - 1];
	g_buff = buff;
	int i = 0;
	for (i = 0; i < sizeof(ori_data) - 1; i++) {
		buff[i].pos = i;
		buff[i].data = ori_data[i] - '0';
	}

	my_msort(buff, 0, sizeof(buff) / sizeof(data_pos) - 1);
	//qsort(buff, sizeof(buff) / sizeof(data_pos), sizeof(data_pos), lib_qsort_cmp);

	print_buff(buff, sizeof(buff) / sizeof(data_pos));

	return 0;
}
// 输出数组
void print_buff(data_pos * buff, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		printf("%d:%d ", buff[i].data, buff[i].pos);
	}
	puts("");
}
// 输出数组并添加IJ指针
void print_buff_ij(data_pos * buff, int l, int r) {
	int i = 0;
	for (i = 0; i < data_len; i++) {
		char *lp = "";
		char *rp = "";
		if (l == i) {
			lp = "->";
		}
		if (r == i) {
			rp = "<-";
		}
		printf("%s%d:%d%s ", lp, buff[i].data, buff[i].pos, rp);
	}
	puts("");
}
//输出并高亮局部
void print_buff_hl(data_pos * buff, int l, int r) {
	int i = 0;
	for (i = 0; i < data_len; i++) {
		char *lp = "";
		char *rp = "";
		if (l == i) {
			lp = "\033[42;31m";
		}
		if (r == i) {
			rp = "\033[0m";
		}
		printf("%s%d:%d%s ", lp, buff[i].data, buff[i].pos, rp);
	}
	puts("");
}
//输出并高亮三个位置
void print_buff_lmr(data_pos * buff, int l, int m, int r) {
	int i = 0;
	for (i = 0; i < data_len; i++) {
		char *lp = "";
		char *rp = "";
		if (l == i) {
			lp = "\033[40;31m";
			rp = "\033[0m";
		}
		if (m == i) {
			lp = "\033[40;32m";
			rp = "\033[0m";
		}
		if (r == i) {
			lp = "\033[40;33m";
			rp = "\033[0m";
		}
		printf("%s%d:%d%s ", lp, buff[i].data, buff[i].pos, rp);
	}
	puts("");
}
// 我实现的快排（递归）
void my_qsort(data_pos * buff, int start, int end) {
	if (start >= end) {
		return;
	}
	int i = start;
	int j = end;
	int is_r = 0;
	while (i < j) {
		if (buff[i].data > buff[j].data) {
			swap(buff, i, j);
			is_r = !is_r;
		}
		if (is_r) {
			i++;
		} else {
			j--;
		}
	}
	my_qsort(buff, start, i - 1);
	my_qsort(buff, j + 1, end);
}
// 我实现的归并排序
void my_msort(data_pos * buff, int start, int end) {
	data_pos * tmp_buff = (data_pos *)alloca((end - start + 1) * sizeof(data_pos));
	int len = end - start + 1;
	int l, r, m;
	int i, lp, rp;
	int step = 2;
	
	print_buff(buff, end - start + 1);
	while (step <= len * 2) {
		for (i = start; i <= len; i += step) {
			l = i;
			r = i + step - 1;
			if (r > end) r = end;
			m = l + step / 2;
			if (r + 1 < m) {
				continue;
			}
			print_buff_lmr(buff, l, m, r);
			for (lp = l, rp = m; lp < m || rp <= r;) {
				if (lp >= m || (rp <= r && buff[lp].data > buff[rp].data)) {
					//print_buff_lmr(buff, -1, rp, -1);
					memcpy(tmp_buff + lp - l + rp - m, buff + rp, sizeof(data_pos));
					rp++;
				} else {
					//print_buff_lmr(buff, -1, lp, -1);
					memcpy(tmp_buff + lp - l + rp - m, buff + lp, sizeof(data_pos));
					lp++;
				}
			}
			memcpy(buff + l, tmp_buff, (r - l + 1) * sizeof(data_pos));
			print_buff_hl(buff, l, r);
		}
		step *= 2;
	}
}
// 原生快排用的比较
int lib_qsort_cmp(void * a, void * b) {
	data_pos *pa = a, *pb = b;
	printf("comparing %d, %d\n", pa - g_buff, pb - g_buff);
	return (int)(pa->data - pb->data);
}
// 交换两个元素
void swap(data_pos * buf, int a, int b) {
	//printf("swaping %d, %d\n", buf[a].data, buf[b].data);
	data_pos tmp;
	memcpy(&tmp, buf + a, sizeof(data_pos));
	memcpy(buf + a, buf + b, sizeof(data_pos));
	memcpy(buf + b, &tmp, sizeof(data_pos));
}

