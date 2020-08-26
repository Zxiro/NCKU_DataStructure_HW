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
				right = down = this;//�w�]�ȫ��V����
				next = this;
			}
			else data = *b;
		}
	private://��@�`�I���㦳�T�ӫ��� ���V�k �U �P�U�@��  �䤤�k�U�O�Ψӳs����@�C�Φ檺�`�I
		bool head_or_not;//�w�q�O�_���}�Y
		MatrixNode *right, *down;
		union {
			MatrixNode *next;
			Data data;
		};
};
istream& operator>>(istream& input, Matrix& mat) {
	Data d;
	input >> d.row >> d.col;
	d.val = 0;//��l�Ƭ�0
	int pt = max(d.row, d.col);
	mat.headnode = new MatrixNode(false, &d);//�ŧi�@�Ӥ����}�Y���`�I
	mat.col = d.col;
	mat.row = d.row;
	if (pt == 0) {//wrong input?
		mat.headnode->right = mat.headnode;
		return input;
	}
	MatrixNode **head = new MatrixNode*[pt];//�إߤ@�ӫ��а}�C
	for (int i = 0; i < pt; i++) {
		head[i] = new MatrixNode(true, NULL);//��l��pt��node pointer
	}
	Data in;
	MatrixNode*last = head[0];//�̫�@�Ӹ`�I
	for (int i = 0; i < d.row; i++) {
		last = head[i];//�q�C�}�linput
		for (int j = 0; j < d.col; j++) {
			input >> in.val;
			if (in.val != 0) {
				in.col = j;
				in.row = i;
				last->right = new MatrixNode(false, &in);//����0���snode�����̫�@��node
				last = last->right;//��Jlinked list�����̫�@��
				head[j]->next = head[j]->next->down = last;//�쥻next���V����
			}
			//else if(in.val==0){}
		}
		last->right = head[i];//i�C�����s�� ���^����
	}
	for (int i = 0; i < pt; i++) {
		head[i]->next->down = head[i];//���^�ۤv
	}
	for (int i = 0; i < pt - 1; i++) {
		head[i]->next = head[i + 1];//���V�U�@��
	}
	head[pt - 1] = mat.headnode;
	mat.headnode->right = head[0];//�Y���s�_��
	delete[]head;//����
	return input;
};
ostream& operator<<(ostream& out, const Matrix&mat) {
	int currow=0, curcol=0;
	MatrixNode *node = mat.headnode->right;
	if (mat.row == 0 || mat.col == 0) return out;
	while (true) {
		if (node->right == node) {//�Y���^����
			for (int i = curcol; i < mat.col; i++) {
				 out << "0\t";//�ѤU��0
			}
			curcol = mat.col;
		}
		else {
			if (node->right->data.col == curcol) {
				out << node->right->data.val<<"\t";//�x�}��
				node = node->right;//���V�U�@��
				curcol++;
			}
			else {
				curcol++;
				out << "0\t";//�S���x�}�ȸ�0
			}
		}
		if (curcol == mat.col) {
			curcol = 0;
			currow++;//���浲�����U�@�C
			node = node->right;
			node = node->next;//���V�U�@�`�I
			out << endl;
		}if (currow == mat.row) {//�M������
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
		mat.col=mu.col = m.col;//�x�}���k
		int pt = max(mu.row, mu.col);
		mat.headnode = new MatrixNode(false, &mu);
		if (pt == 0) {
			mat.headnode->right = mat.headnode;
			return mat;
		}
		MatrixNode **head = new MatrixNode*[pt];
		for (int i = 0; i < pt; i++) {
			head[i] = new MatrixNode(true, NULL);//��l��i��node pointer
		}/*�P�W�z�ۦP*/
		MatrixNode *last = head[0];//�إ߶}�Y����
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
						break;//�Y���s�_�� ����
					}
					if (mu1->right->data.col < mu2->down->data.row) {
						mu1 = mu1->right;//�V�k���S�����
					}
					if (mu1->right->data.col > mu2->down->data.row) {
						mu2 = mu2->down;//�V�U���S�����
					}
					if (mu1->right->data.col == mu2->down->data.row) {
						muval = muval + (mu1->right->data.val *mu2->down->data.val);//�ۭ�
						mu2 = mu2->down;
						mu1 = mu1->right;//��ӯx�}�ҩ��U�䤸��
					}
				}
				mu.row = i; mu.col = j; mu.val = muval;
				if (muval != 0) {//���k���� �N�s�Jhead�x�}
					last=last->right = new MatrixNode(false, &mu);
					head[j]->next = head[j]->next->down = last;
				}
				mu2_head=mu2 = mu2->next;
			}
			last->right = head[i];//���^�h
			this->headnode->right=mu1= mu1->next;
			mu2 = mu2_head = m.headnode->right;
		}
		for (int i = 0; i < pt; i++) {
			head[i]->next->down = head[i];//�P�W�z�ۦP�ѳ̫�@�ӳs���ܲĤ@��(����V)
		}
		for (int i = 0; i < pt - 1; i++) {
			head[i]->next = head[i + 1]; // �P���䪺�۳s
		}
		head[pt - 1]->next = mat.headnode;// �P�W�z�ۦP�ѳ̫�@�ӳs���ܲĤ@��(���V)
		mat.headnode->right = head[0];//���^�ۤv
		delete[] head;//����Ŷ�
		return mat;
	}

}
int main(void) {
	Matrix m1, m2, ans;
	cin >> m1;
	cout << "�п�J�ĤG�յ}���x�}" << endl;
	cin >> m2;
	ans = m1.mult(m2);
	cout <<"Answer Matrix"<< endl;
	cout << ans;
	system("pause");
	return 0;
}
	
	
	