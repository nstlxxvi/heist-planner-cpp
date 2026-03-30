#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <functional>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <compare>

struct Vault {
  unsigned value;
  unsigned pebbles;
  std::vector<std::pair<unsigned, unsigned>> missing_connections;
};

struct UnlockingSequence {
  unsigned vault_id;
  std::vector<bool> moved_pebbles;
};


std::optional<std::vector<bool>> open_vault(const Vault& vault)
{
  unsigned N = vault.pebbles;
  if (N % 2 != 0) return std::nullopt;

  std::vector<std::vector<unsigned>> adj(N);
  for (auto [x, y] : vault.missing_connections)
  {
    adj[x].push_back(y);
    adj[y].push_back(x);
  }

  std::vector<int> color(N, -1);
  std::vector<std::vector<unsigned>> comp;
  int curr_color = 0;

  for (unsigned i = 0; i < N; i++)
  {
    if (color[i] == -1)
    {
      std::vector<unsigned> curr_comp;
      std::queue<unsigned> q;

      q.push(i);

      color[i] = curr_color;

      while (!q.empty())
      {
        unsigned x = q.front();
        q.pop();
        curr_comp.push_back(x);
        for(unsigned y : adj[x])
        {
          if (color[y] == -1)
          {
            if (color[y] == -1)
            {
              color[y] = curr_color;
              q.push(y);
            }
          }
        }
      }
      comp.push_back(curr_comp);
      curr_color++;
    }
  }

  unsigned target = N/2;
  unsigned comp_count = comp.size();
  std::vector<std::vector<int>> dp(comp_count + 1, std::vector<int>(target + 1, -1));
  dp[0][0] = 1;

  for (unsigned i = 0; i< comp_count; i++)
  {
    unsigned size = comp[i].size();

    for (unsigned j = 0; j <= target; j++)
    {
      if (dp[i][j] != -1)
      {
        dp[i+1][j] = 0;
        if (j + size <= target) dp[i+1][j+size] = 1;
      }
    }
  }

  if (dp[comp_count][target] == -1) return std::nullopt;

  std::vector<bool> moved(N, false);
  unsigned curr_w = target;

  for (int i = comp_count; i > 0; i--)
  {
    if (dp[i][curr_w] == 1)
    {
      unsigned size = comp[i-1].size();
      for (unsigned node : comp[i-1])
      {
        moved[node] = true;
      }
      curr_w -= size;
    }
  }
  return moved;
}
std::vector<UnlockingSequence> plan_heist(
    const std::vector<Vault>& vaults,
    unsigned transition,
    unsigned max_time
) {
    struct ValidVault
    {
      unsigned id;
      int weight;
      unsigned value;
      std::vector<bool> moved;
    };

  std::vector<ValidVault> items;
  for (unsigned i = 0; i < vaults.size(); i++)
  {
    if (auto r = open_vault(vaults[i]))
    {
      int cost = (vaults[i].pebbles / 2) + transition;
      items.push_back({i, cost, vaults[i].value, std::move(*r)});
    }
  }

  if (items.empty()) return {};

  int s =items.size();
  int lim = max_time + transition;

  std::vector<std::vector<int>> dp(s + 1, std::vector<int>(lim + 1, -1));
  dp[0][0] = 0;

  for (int i = 1; i <= s; i++)
  {
    int w = items[i-1].weight;
    int v = items[i-1].value;
    for (int j = 0; j <= lim; j++)
    {
      dp[i][j] = dp[i-1][j];
      if (j >= w && dp[i-1][j - w] != -1)
      {
        dp[i][j] = std::max(dp[i][j], dp[i-1][j-w] + v);
      }
    }
  }

  int res_v = 0, res_w = 0;
  for (int j = 0; j <= lim; j++)
  {
    if (dp[s][j] > res_v)
    {
      res_v = dp[s][j];
      res_w = j;
    }
  }

  std::vector<UnlockingSequence> result;
  int curr_w = res_w;
  for (int i = s; i > 0; i--)
  {
    int w = items[i-1].weight;
    int v = items[i-1].value;

    if (curr_w >= w && dp[i-1][curr_w - w] != -1 && dp[i][curr_w] == dp[i-1][curr_w-w] + v)
    {
      result.push_back({items[i-1].id, items[i-1].moved});
      curr_w -= w;
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

int main() {
  std::vector<Vault> vaults = {
    {100, 4, {{0,1}}},
    {200, 6, {{1,2}, {3,4}}}
  };

  auto result = plan_heist(vaults, 2, 10);

  std::cout << "Selected vaults:\n";
  for (const auto& v : result) {
    std::cout << "Vault " << v.vault_id << "\n";
  }

  return 0;
}