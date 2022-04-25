int frows, fcols;
int ppn, plen;
int mrate = 2;
int c_index;
bool found = false;
uchar **best;
uchar ***data, ***ndata;
int *mpool;
 
Species *ppl;

struct Species
{
public:
    int fit;
    int _rw, _cl;
    int index;
 
    void setFit()
    {
        int cnt = 0;
        for (int i = 0; i < _rw; i++)
        {
            for (int j = 0; j < _cl; j++)
            {
                if (data[index][i][j] == best[i][j])
                    cnt++;
            }
        }
        fit = 1000 * cnt / (_rw * _cl);
        fit = (fit * fit) / 1000;
    }
 
    void allocate(int rw, int cl)
    {
        _rw = rw;
        _cl = cl;
        index = c_index;
        for (int i = 0; i < _rw; i++)
            for (int j = 0; j < _cl; j++)
                data[c_index][i][j] = rand() % 256;
        c_index++;
    }
 
    void copyImg(Mat m)
    {
        for (int i = 0; i < m.rows; i++)
        {
            uchar *lin = m.ptr(i);
            for (int j = 0; j < m.cols; j++)
            {
                lin[j] = (uchar)data[index][i][j];
            }
        }
    }
 
    void mutate()
    {
        for (int i = 0; i < _rw; i++)
        {
            for (int j = 0; j < _cl; j++)
            {
                if (rand() % 100 < mrate)
                    ndata[index][i][j] = rand() % 256;
            }
        }
    }
};

bool sortRule(Species a, Species b)
{
    return a.fit > b.fit;
}
 
void breed(int place, Species a, Species b)
{
    for (int i = 0; i < frows; i++)
    {
        for (int j = 0; j < fcols; j++)
        {
            if ((data[a.index][i][j] == best[i][j]) || (data[b.index][i][j] == best[i][j]))
            {
                if (rand() % 10 == 0)
                    ndata[place][i][j] = best[i][j];
                else
                    goto branch;
            }
            else
            {
            branch:
                if (rand() % 2 == 0)
                    ndata[place][i][j] = data[a.index][i][j];
                else
                    ndata[place][i][j] = data[b.index][i][j];
            }
        }
    }
}

srand(time(NULL));
 
Mat target, proc;
target = imread("logoteknik.jpeg", 0);
namedWindow("Display Image", WINDOW_AUTOSIZE);
 
proc = target;
frows = target.rows;
fcols = target.cols;
 
best = new uchar *[frows];
for (int i = 0; i < frows; i++)
{
    best[i] = new uchar[fcols];
}
for (int i = 0; i < frows; i++)
{
    uchar *lin = target.ptr(i);
    for (int j = 0; j < fcols; j++)
    {
        best[i][j] = (uchar)lin[j];
    }
}
std::cin >> ppn;
mpool = new int[ppn * (ppn + 1) / 2];
 
data = new uchar **[ppn + 10];
for (int i = 0; i < ppn + 10; i++)
{
    data[i] = new uchar *[frows];
    for (int j = 0; j < frows; j++)
        data[i][j] = new uchar[fcols];
}
ndata = new uchar **[ppn + 10];
for (int i = 0; i < ppn + 10; i++)
{
    ndata[i] = new uchar *[frows];
    for (int j = 0; j < frows; j++)
        ndata[i][j] = new uchar[fcols];
}
ppl = new Species[ppn];
for (int i = 0; i < ppn; i++)
{
    ppl[i].allocate(frows, fcols);
    ppl[i].setFit();
}
while (!found)
{
    for (int i = 0; i < ppn; i++)
    {
        ppl[i].setFit();
        if (ppl[i].fit > 990)
            found = true;
    }
    std::sort(ppl, ppl + ppn, sortRule);
    std::cout << ppl[0].fit << '\n';
    ppl[0].copyImg(proc);
    imshow("Display Image", proc);
    waitKey(5);
 
    if (found)
        break;
 
    plen = 0;
    //create mating pool
    for (int i = 0; i < ppn; i++)
    {
        for (int j = 0; j < ppn - i; j++)
        {
            mpool[plen] = i;
            plen += 1;
        }
    }
    for (int i = 0; i < ppn; i++)
    {
        int a = rand() % plen, b = rand() % plen;
        breed(i, ppl[mpool[a]], ppl[mpool[b]]);
    }
    for (int i = 0; i < ppn; i++)
        ppl[i].mutate();
    swap(data, ndata);
}
delete data;
delete ndata;
return 0;
}