#ifndef RUNNABLE_H
#define RUNNABLE_H

class Runnable
{
public:
    virtual ~Runnable() {}
    virtual void run() = 0;

    inline bool autoDelete() const { return _autoDelete; }
    inline void setAutoDelete(bool autoDelete) { _autoDelete = autoDelete; }

private:
    bool _autoDelete {true};
};

#endif // RUNNABLE_H
