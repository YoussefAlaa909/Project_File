
/*================ AVAIL NODE CLASS =================*/

class AvailNode
{
public:
    int offset;
    short size;

    AvailNode() : offset(0), size(0) {}
    AvailNode(int offset, short size) : offset(offset), size(size) {}

    int getOffset() const { return offset; }
    short getSize() const { return size; }

    bool operator<(const AvailNode &a) const
    {
        return size < a.size;
    }
};