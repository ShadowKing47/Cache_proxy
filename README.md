## Caching Proxy Server

Cache Proxy is a simple caching proxy server implemented in C++. 
It serves as a basic solution for caching HTTP requests and responses using a card in memory to store the cache.

## Limitations

### Memory Usage:

Since the cache is stored in memory, it is limited by the available RAM. 
For large-scale use cases, consider implementing a more sophisticated caching mechanism that can handle larger datasets or persist the cache to disk.

### No Cache Eviction Policy:

This implementation does not include a cache eviction policy. The cache will grow indefinitely, which could lead to memory exhaustion in long-running scenarios. 
Adding an eviction policy like LRU (Least Recently Used) would be a recommended enhancement.

## Usage

### Start the server:

caching-proxy --port <number> --origin <url>

### To clear the cache:

caching-proxy --clear-cache

### e.g:

caching-proxy --port 3000 --origin http://dummyjson.com

## Extras

This Repo serves as a solution to https://roadmap.sh/projects/caching-server
