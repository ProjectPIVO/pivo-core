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

#ifndef PIVO_GRAPH_H
#define PIVO_GRAPH_H

#include <set>
#include <map>
#include <stack>
#include <stdint.h>

typedef uint32_t nodeid_t;

// set of node identificators
typedef std::set<nodeid_t> NodeSet;
// key = node ID, value = set of adjacent nodes
typedef std::map<nodeid_t, NodeSet> AdjacencyMap;

// key = node ID, value = component, in which the node belongs (should always be >= 0, because node itself
// is considered component in graph theory
typedef std::map<nodeid_t, int> NodeComponentMap;
// key = component ID, value = set of nodes belonging to component
typedef std::map<int, NodeSet> ComponentNodeMap;

// container for found (strongly connected) components in graph
struct ComponentGraphContainer
{
    // map of nodeid-componentid
    NodeComponentMap nodeComponents;
    // map of componentid-list of nodes
    ComponentNodeMap components;

    void clear();

    bool isInSameComponent(nodeid_t a, nodeid_t b);
};

// graph containing just IDs - we will use it for function call graph, etc., so IDs will mean function ID, etc.
class Graph
{
    public:
        // non-parametric constructor
        Graph();
        // adds node to graph
        void addNode(nodeid_t id);
        // adds oriented edge to graph (from src to dst)
        void addEdge(nodeid_t src, nodeid_t dst);

        // is node present in this graph?
        bool hasNode(nodeid_t id);
        // is specific oriented edge present in graph?
        bool isAdjacent(nodeid_t src, nodeid_t dst);

        // retrieves set of adjacent nodes
        NodeSet* getAdjacentNodes(nodeid_t id);
        // fills supplied node set with all nodes present in graph
        void getNodes(NodeSet* target);

        // resets traversal state for all available traversal algorithms
        void resetTraversalState();

        // finds all strongly connected components and stores them into componentContainer
        void findSCC();

        // public component container
        ComponentGraphContainer componentContainer;

    protected:
        // expands node using DFS within Tarjan's algorithm for finding strongly connected components
        void expandSCC(nodeid_t id);

        // storage for oriented edges (and eventually nodes)
        AdjacencyMap m_adjacency;

        // traversal maps - declare them here, so we won't need to pass them via stack
        // with every function call (recursion, etc.)

        // current discovery time
        int m_discoveryTime;
        // current component ID
        int m_lastComponentId;
        // discovery state - key = source node, value = discovered from node
        std::map<nodeid_t, nodeid_t> m_discovered;
        // currently being traversed (in stack, queue, etc.), this does not mean the node is closed
        std::set<nodeid_t> m_traversed;
        // node with minimal discovery time, that can be reached from key node
        std::map<nodeid_t, nodeid_t> m_earlyVisit;
        // stack used for graph traversal
        std::stack<nodeid_t> m_traversalStack;
};

#endif
