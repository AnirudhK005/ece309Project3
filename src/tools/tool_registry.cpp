// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"

ToolRegistry::ToolRegistry() : buckets(8){};
ToolRegistry::~ToolRegistry() = default;

std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    std::uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : key) { h ^= c; h *= 1099511628211ULL; }
    return static_cast<std::size_t>(h);
}

bool ToolRegistry::add(std::unique_ptr<Tool> newTool) {
    if(newTool == nullptr){
        return false;
    }

    Tool* presentTool = find(newTool->name());
    if(presentTool != nullptr){
        return false;
    }

    if(2 * (count + 1) > buckets.size()){
        std::vector<std::unique_ptr<Tool>> newBucket(buckets.size() * 2);
        for(std::size_t i = 0; i < buckets.size(); i++){
            if(buckets[i]){
                std::size_t j = hash(buckets[i]->name()) % newBucket.size();
                while(newBucket[j]){
                    j = (j + 1) % newBucket.size();
                }
                newBucket[j] = std::move(buckets[i]);
            }
        }
        buckets = std::move(newBucket);
    }

    std::size_t i = hash(newTool->name()) % buckets.size();
    std::size_t counter = 0;
    while(counter++ < buckets.size()){
        if(!buckets[i]){
            buckets[i] = std::move(newTool);
            count++;
            return true;
        }
        i = (i + 1) % buckets.size();
    }
    return false;
    
}

Tool* ToolRegistry::find(std::string_view toolName) const {
    std::size_t i = hash(toolName) % buckets.size();
    std::size_t counter = 0;
    while(buckets[i] && counter++ < buckets.size()){
        if(buckets[i]->name() == toolName){
            return buckets[i].get();
        }
        i = (i + 1) % buckets.size();
    }
    return nullptr;
}

std::size_t ToolRegistry::size() const noexcept {
    return count;
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& fn) const {
    for(std::size_t i = 0; i < buckets.size(); i++){
        if(buckets[i]){
            fn(*buckets[i]);
        }
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets.size();
}

double ToolRegistry::load_factor() const noexcept {
    double presentCount = count;
    double total = buckets.size();
    return presentCount / total;
}

std::size_t ToolRegistry::max_probe_length() const {
    std::size_t max = 0;
    std::size_t size = buckets.size();
    for(std::size_t i = 0; i < size; i++){
        if(buckets[i]){
            std::size_t j = hash(buckets[i]->name()) % size;
            std::size_t counter = 0;
            while(buckets[j] && counter++ < size){
                if(buckets[j]->name() == buckets[i]->name()){
                    break;
                }
                j = (j + 1) % size;
            }
            if(counter > max){
                max = counter;
            }
        }
    }
    return max; 
}