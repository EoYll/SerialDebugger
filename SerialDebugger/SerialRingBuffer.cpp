// SerialRingBuffer.cpp
#include "SerialRingBuffer.h"

SerialRingBuffer::SerialRingBuffer(int capacity) :
    m_capacity(capacity),
    m_buffer(new char[capacity]),
    m_readIndex(0),
    m_writeIndex(0),
    m_freeSpace(capacity),   // 初始空闲空间 = 容量
    m_usedSpace(0)           // 初始已用空间 = 0
{
    // 移除 m_freeSpace.acquire(capacity);
}

int SerialRingBuffer::write(const char* data, int length) {
    if (length <= 0) return 0;
    QMutexLocker locker(&m_mutex);

    int actualWrite = 0;
    while (actualWrite < length) {
        int blockSize = qMin(length - actualWrite,
            qMin(m_capacity - m_writeIndex,
                m_freeSpace.available())); // 当前可用空闲空间

        if (blockSize <= 0) break;

        memcpy(m_buffer + m_writeIndex, data + actualWrite, blockSize);
        actualWrite += blockSize;
        m_writeIndex = (m_writeIndex + blockSize) % m_capacity;

        // 修正信号量操作：
        m_usedSpace.release(blockSize);   // 已用空间增加
        m_freeSpace.acquire(blockSize);    // 空闲空间减少
    }
    return actualWrite;
}

QByteArray SerialRingBuffer::read(int maxLength) {
    QMutexLocker locker(&m_mutex);
    QByteArray result;
    if (m_usedSpace.available() <= 0)
        return result;

    int bytesToRead = qMin(maxLength, m_usedSpace.available());
    result.resize(bytesToRead);

    int read = 0;
    while (read < bytesToRead) {
        int blockSize = qMin(bytesToRead - read, m_capacity - m_readIndex);
        memcpy(result.data() + read, m_buffer + m_readIndex, blockSize);
        read += blockSize;
        m_readIndex = (m_readIndex + blockSize) % m_capacity;
    }

    // 修正信号量操作：
    m_freeSpace.release(bytesToRead);   // 空闲空间增加
    m_usedSpace.acquire(bytesToRead);    // 已用空间减少

    return result;
}

void SerialRingBuffer::clear() {
    QMutexLocker locker(&m_mutex);
    m_readIndex = 0;
    m_writeIndex = 0;

    // 重置信号量：将已用空间转为空闲空间
    int used = m_usedSpace.available();
    if (used > 0) {
        m_freeSpace.release(used);   // 空闲空间增加
        m_usedSpace.acquire(used);    // 已用空间清零
    }
}

int SerialRingBuffer::dataSize() const {
    return m_usedSpace.available(); // 直接返回已用空间信号量
}