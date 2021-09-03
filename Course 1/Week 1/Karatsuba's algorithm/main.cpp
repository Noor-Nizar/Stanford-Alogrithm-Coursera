#include <iostream>
#include <vector>

using namespace std;

vector<int> Fix(vector<int> A, int B)
{
    vector<int> Res(B, 0);
    for (int i = B - A.size(); i < B; i++)
        Res[i] = A[i - (B - A.size())];

    return Res;
}

vector<int> Trim(vector<int> A)
{
    int i;
    for (i = 0; i < A.size(); i++)
        if (A[i] != 0)
            break;

    vector<int> Res(A.begin() + i, A.end());
    if (Res.size() == 0)
        return {0};
    return Res;
}

vector<int> padV(vector<int> A, int pe)
{
    int n = A.size();
    vector<int> Res(n + pe);
    int i;
    for (i = 0; i < n; i++)
        Res[i] = A[i];
    for (; i < n + pe; i++)
        Res[i] = 0;

    return Trim(Res);
}

vector<int> SubV(vector<int> A, vector<int> B)
{
    bool AisBigger = true;

    int n = A.size();
    int m = B.size();

    int rN = 0;
    int rM = 0;
    for (; rN < n; rN++)
        if (A[rN] != 0)
            break;
    for (; rM < m; rM++)
        if (B[rM] != 0)
            break;

    if (n - rN < m - rM)
        AisBigger = false;
    else if (n - rN == m - rM)
    {
        for (int i = rN, j = rM; i < A.size(); i++, j++)
        {
            if (B[j] > A[i])
            {
                AisBigger = false;
                break;
            }
            else if (A[i] > B[j])
                break;
        }
    }
    vector<int> Res(max(n, m));

    int k = Res.size() - 1;

    int c = 0;
    int i = n - 1;
    int j = m - 1;
    if (AisBigger)
    {
        vector<int> Bnew(n);
        int pad;
        for (pad = 0; pad < n - m; pad++)
            Bnew[pad] = 0;
        for (pad; pad < n; pad++)
            Bnew[pad] = B[pad - (n - m)];

        for (; i >= 0; i--)
        {
            if (A[i] - Bnew[i] >= 0)
                Res[k - c] = A[i] - Bnew[i];
            else
            {
                Res[k - c] = 10 - (Bnew[i] - A[i]);
                Bnew[i - 1] += 1;
            }
            c++;
        }
    }
    else
    {
        vector<int> Anew(m);
        int pad;
        for (pad = 0; pad < m - n; pad++)
            Anew[pad] = 0;
        for (pad; pad < m; pad++)
            Anew[pad] = A[pad - (m - n)];

        for (; j >= 0; j--)
        {
            if (B[j] - Anew[j] >= 0)
                Res[k - c] = B[j] - Anew[j];
            else
            {
                Res[k - c] = 10 - (Anew[j] - B[j]);
                Anew[j - 1] += 1;
            }
            c++;
        }
    }

    return Trim(Res);
}

vector<int> MultiplyV(vector<int> A, int B)
{
    vector<int> Res(A.size() + 1);
    for (int i = A.size() - 1; i >= 0; i--)
    {
        Res[i + 1] += A[i] * B % 10;
        Res[i] += (A[i] * B) / 10;
        if (Res[i + 1] > 9)
        {
            Res[i] += Res[i + 1] / 10;
            Res[i + 1] = Res[i + 1] % 10;
        }
    }

    return Trim(Res);
}

vector<int> addV(vector<int> A, vector<int> B)
{
    int n = A.size();
    int m = B.size();
    vector<int> Res(max(n, m) + 1);
    int k = Res.size() - 1;

    int c = 0;
    int i = n - 1;
    int j = m - 1;
    for (; i >= 0 && j >= 0; i--, j--)
    {
        Res[k - c] += (A[i] + B[j]) % 10;
        Res[k - c - 1] += (A[i] + B[j]) / 10;
        if (Res[k - c] > 9)
        {
            Res[k - c - 1]++;
            Res[k - c] = 0;
        }
        c++;
    }
    while (i-- >= 0)
        Res[k - c++] += A[i + 1];

    while (j-- >= 0)
        Res[k - c++] += B[j + 1];

    return Trim(Res);
}

vector<int> Karatsuba(vector<int> n1, vector<int> n2)
{
    if (n1.size() <= 1)
        return MultiplyV(n2, n1[0]);

    else if (n2.size() <= 1)
        return MultiplyV(n1, n2[0]);

    int n1s = n1.size();
    int n2s = n2.size();

    if (n1s > n2s)
        n2 = Fix(n2, n1s);
    else
        n1 = Fix(n1, n2s);

    int midA = n1.size() / 2;
    vector<int> A(n1.begin(), n1.begin() + midA);
    vector<int> B(n1.begin() + midA, n1.end());

    vector<int> C(n2.begin(), n2.begin() + midA);
    vector<int> D(n2.begin() + midA, n2.end());

    vector<int> AC = Karatsuba(A, C);
    vector<int> BD = Karatsuba(B, D);
    vector<int> AD_BC2 = SubV(SubV(Karatsuba(addV(A, B), addV(C, D)), AC), BD);

    vector<int> Res((n1.size() - A.size()) * 2 + 1);
    Res = addV(addV(padV(AC, (n1.size() - A.size()) * 2), padV(AD_BC2, n1.size() - A.size())), BD);
    return Trim(Res);
}

int main()
{
    vector<int> arr1(1, 0);
    vector<int> arr2(1, 0);
    string s1 = "3141592653589793238462643383279502884197169399375105820974944592";
    string s2 = "2718281828459045235360287471352662497757247093699959574966967627";

    for (int i = 0; i < s1.size(); i++)
        arr1.push_back(s1[i] - '0');
    for (int i = 0; i < s2.size(); i++)
        arr2.push_back(s2[i] - '0');
    vector<int> Res = Karatsuba(arr1, arr2);
    for (int i = 0; i < Res.size(); i++)
        cout << Res[i];
    cout << endl;
    return 0;
}