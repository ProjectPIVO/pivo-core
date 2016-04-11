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

#include "General.h"
#include "Graph.h"

#include <algorithm>

void ComponentGraphContainer::clear()
{
    components.clear();
    nodeComponents.clear();
}

bool ComponentGraphContainer::isInSameComponent(nodeid_t a, nodeid_t b)
{
    return (nodeComponents[a] == nodeComponents[b]);
}

Graph::Graph()
{
    //
}

void Graph::addNode(nodeid_t id)
{
    if (m_adjacency.find(id) != m_adjacency.end())
        return;

    m_adjacency[id] = NodeSet();
}

void Graph::addEdge(nodeid_t src, nodeid_t dst)
{
    if (m_adjacency.find(src) == m_adjacency.end())
        addNode(src);
    if (m_adjacency.find(dst) == m_adjacency.end())
        addNode(dst);

    m_adjacency[src].insert(dst);
}

bool Graph::hasNode(nodeid_t id)
{
    return (m_adjacency.find(id) != m_adjacency.end());
}

bool Graph::isAdjacent(nodeid_t src, nodeid_t dst)
{
    return (m_adjacency[src].find(dst) != m_adjacency[src].end());
}

NodeSet* Graph::getAdjacentNodes(nodeid_t id)
{
    if (m_adjacency.find(id) == m_adjacency.end())
        return nullptr;

    return &m_adjacency[id];
}

void Graph::getNodes(NodeSet* target)
{
    std::transform(m_adjacency.begin(), m_adjacency.end(),
                   std::inserter(*target, target->begin()),
                   [](const AdjacencyMap::value_type& pair) { return pair.first; });
}

void Graph::findSCC()
{
    // at first, reset traversal state
    resetTraversalState();

    // clear component container
    componentContainer.components.clear();
    componentContainer.nodeComponents.clear();

    // and expand DFS from all nodes, that has not been discovered yet
    for (AdjacencyMap::iterator itr = m_adjacency.begin(); itr != m_adjacency.end(); ++itr)
        if (m_discovered.find(itr->first) == m_discovered.end())
            expandSCC(itr->first);
}

void Graph::expandSCC(nodeid_t id)
{
    nodeid_t adj;

    m_discoveryTime++;

    m_discovered[id] = m_discoveryTime;
    m_earlyVisit[id] = m_discoveryTime;

    // store node to stack
    m_traversalStack.push(id);
    m_traversed.insert(id);

    // go through all adjacent nodes
    for (NodeSet::iterator itr = m_adjacency[id].begin(); itr != m_adjacency[id].end(); ++itr)
    {
        adj = *itr;
        // if the node is not yet discovered, proceed to visit
        if (m_discovered.find(adj) == m_discovered.end())
        {
            // visit and expand DFS from this node
            expandSCC(adj);
            // and determine common ancestor - if there is such ancestor, they probably belong to
            // one component
            m_earlyVisit[id] = nmin(m_earlyVisit[id], m_earlyVisit[adj]);
        }
        // if the node was discovered, but it's still on stack, that means we detected loop during
        // DFS traversal (expanded node reached some of its ancestors)
        else if (m_traversed.find(adj) != m_traversed.end())
        {
            // assign the lowest of earlyVisit and dicoveredFrom values
            m_earlyVisit[id] = nmin(m_earlyVisit[id], m_discovered[adj]);
        }
    }

    // we found head node of component (earlyVisit still reflects original discovery time)
    if (m_earlyVisit[id] == m_discovered[id])
    {
        // increment component counter
        m_lastComponentId++;

        // while there's something on stack, pop it and assign it to current component
        do
        {
            adj = m_traversalStack.top();

            componentContainer.components[m_lastComponentId].insert(adj);
            componentContainer.nodeComponents[adj] = m_lastComponentId;

            m_traversalStack.pop();
            m_traversed.erase(adj);
        }
        while (adj != id);
    }
}

void Graph::resetTraversalState()
{
    m_discoveryTime = 0;
    m_lastComponentId = 0;
    m_discovered.clear();
    m_traversed.clear();
    m_earlyVisit.clear();
    // swap with clean stack - clear current
    std::stack<nodeid_t>().swap(m_traversalStack);
}
