/*================ PRIMARY INDEX CLASS =================*/

class PrimaryIndex
{
private:
    int id;
    int offset;

public:
    PrimaryIndex() : id(0), offset(0) {}
    PrimaryIndex(int id, int offset) : id(id), offset(offset) {}

    int getId() const { return id; }
    int getOffset() const { return offset; }

    bool operator<(const PrimaryIndex &p) const
    {
        return id < p.id;
    }
};