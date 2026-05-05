/*================ PRIMARY INDEX CLASS =================*/

class ProfPrimaryIndex
{
private:
    int id;
    int offset;

public:
    ProfPrimaryIndex() : id(0), offset(0) {}
   ProfPrimaryIndex(int id, int offset) : id(id), offset(offset) {}

    int getId() const { return id; }
    int getOffset() const { return offset; }

    bool operator<(const ProfPrimaryIndex &p) const
    {
        return id < p.id;
    }
};