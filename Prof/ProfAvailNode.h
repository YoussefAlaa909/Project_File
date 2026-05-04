
/*================ AVAIL NODE CLASS =================*/

class ProfAvailNode
{
public:
    int offset;
    short size;

    ProfAvailNode() : offset(0), size(0) {}
    ProfAvailNode(int offset, short size) : offset(offset), size(size) {}

    int getOffset() const { return offset; }
    short getSize() const { return size; }

    bool operator<(const AvailNode &a) const
    {
        return size < a.size;
    }
};