#ifndef TAPE_HPP
#define TAPE_HPP

#include <QIODevice>
#include <QString>
#include <vector>

class Tape
{
public:
    using index_t = long long int;

    Tape();

    void setValue(index_t index, bool value);
    bool getValue(index_t index) const;

    void setHead(index_t head);
    index_t getHead() const;

    void writeOnHead(bool value);
    bool readOnHead() const;

    void saveToDevice(QTextStream &io) const;
    void loadFromDevice(QTextStream &io);

    void clear();

private:
    std::vector<bool> m_positive;
    std::vector<bool> m_negative;
    index_t m_head = 0;
};

#endif
