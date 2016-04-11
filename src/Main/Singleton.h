/**
 * Copyright (C) 2016 Martin Ubl <http://pivo.kennny.cz>
 *
 * This file is part of PIVO suite.
 *
 * PIVO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIVO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIVO. If not, see <http://www.gnu.org/licenses/>.
 **/

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
