#include <map>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

/**************** Basic Graph Definition ****************/
struct edge{
    long long u, v, t, next;
    edge() {}
    edge(long long _u, long long _v, long long _t, long long _next) : u(_u), v(_v), t(_t), next(_next) {}
};
std::vector<edge> graph;
std::map<long long, long long> hash_to_id, id_to_pointer;


/**************** Declaretions for Building Graph ****************/
long long convert_hash_to_id(long long hash);
void add_edge(long long txid, std::vector<long long>& in_list, std::vector<long long>& out_list);
void build_graph(std::string txinfile, std::string txoutfile);


/**************** Functions for Statistics ****************/
void stat1() {
    std::cout << "## Statistic1: Basics" << std::endl;

    int vertex_cnt = hash_to_id.size();
    std::cout << "Graph Size: " << vertex_cnt << " vertexes, "
              << graph.size() << " edges" << std::endl;

    std::vector<int> out_degree_cnt(vertex_cnt, 0), in_degree_cnt(vertex_cnt, 0);
    for (auto it : graph) {
        out_degree_cnt[it.u]++;
        in_degree_cnt[it.v]++;
    }

    std::sort(out_degree_cnt.begin(), out_degree_cnt.end());
    std::sort(in_degree_cnt.begin(), in_degree_cnt.end());
    
    int idx50 = vertex_cnt * 0.5;
    int idx90 = vertex_cnt * 0.9;
    int idx95 = vertex_cnt * 0.95;
    int idx99 = vertex_cnt * 0.99;
    int idx999 = vertex_cnt * 0.999;
    std::cout << "### out degree:" << std::endl;
    std::cout << "50% degree:\t" << out_degree_cnt[idx50] << std::endl
              << "90% degree:\t" << out_degree_cnt[idx90] << std::endl
              << "95% degree:\t" << out_degree_cnt[idx95] << std::endl
              << "99% degree:\t" << out_degree_cnt[idx99] << std::endl
              << "99.9% degree:\t" << out_degree_cnt[idx999] << std::endl
              << "100% degree:\t" << out_degree_cnt[vertex_cnt - 1] << std::endl;

    std::cout << "### in degree:" << std::endl;
    std::cout << "50% degree:\t" << in_degree_cnt[idx50] << std::endl
              << "90% degree:\t" << in_degree_cnt[idx90] << std::endl
              << "95% degree:\t" << in_degree_cnt[idx95] << std::endl
              << "99% degree:\t" << in_degree_cnt[idx99] << std::endl
              << "99.9% degree:\t" << in_degree_cnt[idx999] << std::endl
              << "100% degree:\t" << in_degree_cnt[vertex_cnt - 1] << std::endl;    
}

void stat2() {
    // Motif1: (b, c):1, (c, a):2, (a, b):3 cycle
    // Motif2: (c, a):1, (b, c):2, (a, b):3 cycle
    // Motif3: (a, b):1, (b, a):2, (a, b):3 cycle

    std::cout << "## Statistic2: 3 cycle motifs" << std::endl;
    int vertex_cnt(id_to_pointer.size());
    int edge_cnt(graph.size());
    long long counter1(0), counter2(0), counter3(0);
    std::vector<int> id_to_pointer_local(vertex_cnt, -1);

    long long stat_semi = edge_cnt / 10, stat_step = 0, stat_perc = 0;

    for (int i = 0; i < edge_cnt; i++) {
        long long a = graph[i].u, b = graph[i].v;
        if (a == b)
            continue;

        // from b to c
        for (int j = id_to_pointer_local[b]; j != -1; j = graph[j].next) {
            long long c = graph[j].v;
            if (b == c)
                continue;

            if (a == c) {
                // Motif3
                for(int k = graph[j].next; k != -1; k = graph[k].next) {
                    if (graph[k].v == b)
                        counter3++;
                }
            } else {
                // Motif1&2
                for (int k = id_to_pointer_local[c]; k != -1; k = graph[k].next) {
                    if (graph[k].v == a && k > j)
                        // a new Motif1 found!
                        counter1++;
                    if (graph[k].v == a && j > k)
                        // a new Motif2 found!
                        counter2++;
                }
            }
        }

        // add this edge to graph
        id_to_pointer_local[a] = i;

        // print progess
        if (i >= stat_step + stat_semi) {
            stat_step += stat_semi;
            stat_perc += 1;
            //std::cout << "Motif1&2 processing: " << (stat_perc * 10) << "%" << std::endl;
        }
    }

    std::cout << "Motif1 count: " << counter1 << std::endl;
    std::cout << "Motif2 count: " << counter2 << std::endl;
    std::cout << "Motif3 count: " << counter3 << std::endl;

}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: ./stat1 <dataset_id> <date_id>" << std::endl;
        return 0;
    }

    std::string txinfile = std::string("data") + argv[1] + "/txin/txin." + argv[1] + "." + argv[2];
    std::string txoutfile = std::string("data") + argv[1] + "/txout/txout." + argv[1] + "." + argv[2];


    std::clock_t begin1 = clock();
    build_graph(txinfile, txoutfile);
    stat1();
    std::clock_t begin2 = clock();
    std::cout << "@time for init: " << double(begin2 - begin1) / CLOCKS_PER_SEC << std::endl << std::endl;
    stat2();
    std::clock_t begin3 = clock();
    std::cout << "@time for motif counting: " << double(begin3 - begin2) / CLOCKS_PER_SEC << std::endl << std::endl;

    std::cout << std::endl << std::endl;

    return 0;
}

/**************** Functions for Building Graph ****************/
long long vertex_cnt(0);
long long convert_hash_to_id(long long hash) {
    if (hash_to_id.find(hash) == hash_to_id.end()) {
        // new vertex
        hash_to_id[hash] = vertex_cnt;
        id_to_pointer[vertex_cnt] = -1;
        return vertex_cnt++;
    } else {
        // old vertex
        return hash_to_id[hash];
    }
}

void add_edge(long long txid, std::vector<long long>& in_list, std::vector<long long>& out_list) {
    int in_size = in_list.size();
    int out_size = out_list.size();

    for (int i = 0; i < in_size; i++) {
        for (int j = 0; j < out_size; j++) {
            long long u = convert_hash_to_id(in_list[i]);
            long long v = convert_hash_to_id(out_list[j]);
            
            graph.push_back(edge(u, v, txid, id_to_pointer[u]));
            id_to_pointer[u] = graph.size() - 1;
        }
    }
}

void build_graph(std::string txinfile, std::string txoutfile) {
    std::ifstream ifs_in(txinfile, std::ifstream::in);
    std::ifstream ifs_out(txoutfile, std::ifstream::out);

    long long txin_cnt(0), txout_cnt(0);
    long long txin_total, txout_total;
    ifs_in >> txin_total;
    ifs_out >> txout_total;
    //std::cout << txin_total << " " << txout_total << std::endl;
    
    long long in_txid, in_seq, in_hash, in_amount;
    long long out_txid, out_seq, out_hash, out_amount;

    // first transaction in this time period
    txin_cnt = 1;
    ifs_in >> in_txid >> in_seq >> in_hash >> in_amount;
    txout_cnt = 1;
    ifs_out >> out_txid >> out_seq >> out_hash >> out_amount;

    // initialize in/out vertex list for single transaction
    long long stat_step(1), stat_prev_txid(txout_cnt), stat_semi(txout_total / 10);
    std::vector<long long> seller, buyer;
    for (int txid = std::max(in_txid, out_txid);
         txin_cnt < txin_total && txout_cnt < txout_total;
         txid++) {
        
        // processing transaction with ID txid
        seller.clear();
        while (in_txid <= txid && txin_cnt < txin_total) {
            // there is more to read
            if (in_txid == txid && in_hash != -1)
                seller.push_back(in_hash);
            ifs_in >> in_txid >> in_seq >> in_hash >> in_amount;
            txin_cnt += 1;
        }

        buyer.clear();
        while (out_txid <= txid && txout_cnt < txout_total) {
            // there is more to read
            if (out_txid == txid && out_hash != -1)
                buyer.push_back(out_hash);
            ifs_out >> out_txid >> out_seq >> out_hash >> out_amount;
            txout_cnt += 1;
        }

        add_edge(txid, seller, buyer);

        // print processing state info
        if (stat_prev_txid + stat_semi < txout_cnt) {
            //std::cout << "preprocessing " << (stat_step * 10) << "%" << std::endl;
            stat_step++;
            stat_prev_txid += stat_semi;
        }
    }
    
 Finish:
    ifs_in.close();
    ifs_out.close();
}
