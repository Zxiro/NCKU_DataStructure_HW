#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace::std;
class Matrix;
class MatrixNode;
struct Data{
	int row;
	int col;
	int val;
};
class Matrix {
	friend istream& operator>>(istream&, Matrix&);
	friend ostream& operator<<(ostream&, const Matrix&);//operator overload
private:
	int row, col;
	MatrixNode *headnode;
	public:
		Matrix mult(Matrix &);
		Matrix() {
			row = 0;
			col = 0;
			headnode = NULL;
		}
	
};
class MatrixNode {
	friend class Matrix;
	friend istream& operator>>(istream&, Matrix&);
	friend ostream& operator<<(ostream&, const Matrix&);
	public:
		MatrixNode(bool a, Data *b) {
			head_or_not = a;
			if (a) {
				right = down = this;//預設值指向本身
				next = this;
			}
			else data = *b;
		}
	private://單一節點中具有三個指標 指向右 下 與下一個  其中右下是用來連接單一列或行的節點
		bool head_or_not;//定義是否為開頭
		MatrixNode *right, *down;
		union {
			MatrixNode *next;
			Data data;
		};
};
istream& operator>>(istream& input, Matrix& mat) {
	Data d;
	input >> d.row >> d.col;
	d.val = 0;//初始化為0
	int pt = max(d.row, d.col);
	mat.headnode = new MatrixNode(false, &d);//宣告一個不為開頭之節點
	mat.col = d.col;
	mat.row = d.row;
	if (pt == 0) {//wrong input?
		mat.headnode->right = mat.headnode;
		return input;
	}
	MatrixNode **head = new MatrixNode*[pt];//建立一個指標陣列
	for (int i = 0; i < pt; i++) {
		head[i] = new MatrixNode(true, NULL);//初始化pt個node pointer
	}
	Data in;
	MatrixNode*last = head[0];//最後一個節點
	for (int i = 0; i < d.row; i++) {
		last = head[i];//從列開始input
		for (int j = 0; j < d.col; j++) {
			input >> in.val;
			if (in.val != 0) {
				in.col = j;
				in.row = i;
				last->right = new MatrixNode(false, &in);//不為0的新node成為最後一個node
				last = last->right;//放入linked list中的最後一個
				head[j]->next = head[j]->next->down = last;//原本next指向本身
			}
			//else if(in.val==0){}
		}
		last->right = head[i];//i列完成連結 指回本身
	}
	for (int i = 0; i < pt; i++) {
		head[i]->next->down = head[i];//指回自己
	}
	for (int i = 0; i < pt - 1; i++) {
		head[i]->next = head[i + 1];//指向下一個
	}
	head[pt - 1] = mat.headnode;
	mat.headnode->right = head[0];//頭尾連起來
	delete[]head;//釋放
	return input;
};
ostream& operator<<(ostream& out, const Matrix&mat) {
	int currow=0, curcol=0;
	MatrixNode *node = mat.headnode->right;
	if (mat.row == 0 || mat.col == 0) return out;
	while (true) {
		if (node->right == node) {//若指回本身
			for (int i = curcol; i < mat.col; i++) {
				 out << "0\t";//剩下補0
			}
			curcol = mat.col;
		}
		else {
			if (node->right->data.col == curcol) {
				out << node->right->data.val<<"\t";//矩陣值
				node = node->right;//指向下一個
				curcol++;
			}
			else {
				curcol++;
				out << "0\t";//沒有矩陣值補0
			}
		}
		if (curcol == mat.col) {
			curcol = 0;
			currow++;//此行結束換下一列
			node = node->right;
			node = node->next;//指向下一節點
			out << endl;
		}if (currow == mat.row) {//遍歷結束
			break;
		}
		
	}return out;
}
Matrix Matrix::mult(Matrix&m) {
	Matrix mat;
	Data mu;
	if (this->col != m.row) {
		cout << "Wrong input!" << endl;
		return mat;
	}
	else if (this->col == m.row) {
		mat.row=mu.row =this->row;
		mat.col=mu.col = m.col;//矩陣乘法
		int pt = max(mu.row, mu.col);
		mat.headnode = new MatrixNode(false, &mu);
		if (pt == 0) {
			mat.headnode->right = mat.headnode;
			return mat;
		}
		MatrixNode **head = new MatrixNode*[pt];
		for (int i = 0; i < pt; i++) {
			head[i] = new MatrixNode(true, NULL);//初始化i個node pointer
		}/*與上述相同*/
		MatrixNode *last = head[0];//建立開頭指標
		MatrixNode *mu1 = this->headnode->right;
		MatrixNode *mu2 = m.headnode->right;
		MatrixNode *mu2_head = m.headnode->right;
		for (int i = 0; i < this->row; i++) {
			last = head[i];
			for (int j = 0; j < m.col; j++) {
				int muval = 0;
				while (true)
				{
					if (mu1->right == this->headnode->right || mu2->down == mu2_head) {
						mu1 = this->headnode->right;
						mu2 = mu2_head;
						break;//頭尾連起來 停止
					}
					if (mu1->right->data.col < mu2->down->data.row) {
						mu1 = mu1->right;//向右移沒有對到
					}
					if (mu1->right->data.col > mu2->down->data.row) {
						mu2 = mu2->down;//向下移沒有對到
					}
					if (mu1->right->data.col == mu2->down->data.row) {
						muval = muval + (mu1->right->data.val *mu2->down->data.val);//相乘
						mu2 = mu2->down;
						mu1 = mu1->right;//兩個矩陣皆往下找元素
					}
				}
				mu.row = i; mu.col = j; mu.val = muval;
				if (muval != 0) {//乘法完成 將存入head矩陣
					last=last->right = new MatrixNode(false, &mu);
					head[j]->next = head[j]->next->down = last;
				}
				mu2_head=mu2 = mu2->next;
			}
			last->right = head[i];//接回去
			this->headnode->right=mu1= mu1->next;
			mu2 = mu2_head = m.headnode->right;
		}
		for (int i = 0; i < pt; i++) {
			head[i]->next->down = head[i];//與上述相同由最後一個連接至第一個(直方向)
		}
		for (int i = 0; i < pt - 1; i++) {
			head[i]->next = head[i + 1]; // 與旁邊的相連
		}
		head[pt - 1]->next = mat.headnode;// 與上述相同由最後一個連接至第一個(橫方向)
		mat.headnode->right = head[0];//指回自己
		delete[] head;//釋放空間
		return mat;
	}

}
int main(void) {
	Matrix m1, m2, ans;
	cin >> m1;
	cout << "請輸入第二組稀疏矩陣" << endl;
	cin >> m2;
	ans = m1.mult(m2);
	cout <<"Answer Matrix"<< endl;
	cout << ans;
	system("pause");
	return 0;
}
	
	
	