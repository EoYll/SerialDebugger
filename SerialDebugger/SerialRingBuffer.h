#pragma once
#include <QByteArray>
#include <QSemaphore>
#include <QMutex>

class SerialRingBuffer
{
public:
    explicit SerialRingBuffer(int capacity = 1024); 
    

    ~SerialRingBuffer(); 

    // 写数据到缓冲区（线程安全）
    int write(const char* data, int length); 

    // 从缓冲区读取数据（线程安全）
    QByteArray read(int maxLength);

    // 清空缓冲区
    void clear(); 

    // 当前数据量
    int dataSize()const;

private:
    const int m_capacity;      // 缓冲区总容量
    char* m_buffer;            // 数据存储
    int m_readIndex;           // 读取位置
    int m_writeIndex;          // 写入位置

    QSemaphore m_freeSpace;    // 空闲空间信号量
    QSemaphore m_usedSpace;    // 已用空间信号量
    mutable QMutex m_mutex;    // 线程安全锁
};