struct Pos
{
    int x = -1;
    int y = -1;

    bool IsValid() const { return x != -1 && y != -1; }
    void Clear() {x = -1; y = -1;}

    bool operator==(const Pos& rhs)
    {
        return x == rhs.x && y == rhs.y;
    }
};

std::ostream& operator<<(std::ostream& out, const Pos& pos)
{
    return out << pos.x << "," << pos.y;
}