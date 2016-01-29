#ifndef PIVO_SINGLETON_H
#define PIVO_SINGLETON_H

template <class T>
class Singleton
{
    public:
        static T* instance()
        {
            if (m_instance == nullptr)
                m_instance = new T;

            return m_instance;
        }

    private:
        static T* m_instance;
};

template <class T>
T* Singleton<T>::m_instance = nullptr;

#endif
