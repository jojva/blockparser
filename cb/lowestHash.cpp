
// Compute lowest block hash

#include <algorithm>
#include <util.h>
#include <common.h>
#include <option.h>
#include <callback.h>
#include <sstream>

bool compareTo(const Block *a, const Block *b)
{
    for (int i = kSHA256ByteSize - 1; i >= 0; i--)
    {
        if (a->hash[i] < b->hash[i])
        {
            return true;
        }
        if (a->hash[i] > b->hash[i])
        {
            return false;
        }
    }
    return false;
}

struct LowestHash:public Callback
{
    optparse::OptionParser parser;

    int mNbTop;
    std::vector<const Block*> mTop;

    LowestHash()
    : mNbTop(10)
    , mTop()
    {
        parser
            .usage("")
            .version("")
            .description("Get the lowest hash ever")
            .epilog("")
        ;
    }

    virtual const char                   *name() const         { return "lowestHash";  }
    virtual const optparse::OptionParser *optionParser() const { return &parser;       }
    virtual bool                         needTXHash() const    { return false;         }

    virtual void aliases(std::vector<const char*> &v) const
    {
        v.push_back("low");
    }

    virtual int init(
        int  argc,
        const char *argv[]
    )
    {
        optparse::Values &values = parser.parse_args(argc, argv);
        auto args = parser.args();
        if(args.size() == 2)
        {
            std::istringstream(args[1]) >> mNbTop;
        }
        return 0;
    }

    virtual void start(const Block *start, const Block *end)
    {
        mTop.reserve(end->height);
    }

    virtual void wrapup()
    {
        const auto startTime = usecs();
        std::partial_sort(mTop.begin(), mTop.begin() + mNbTop, mTop.end(), compareTo);
        auto elapsed = usecs() - startTime;
        fprintf(stderr, "lowestHash: sorted top %d elements in %.3f secs\n", mNbTop, elapsed*1e-6);
        uint8_t hash[2*kSHA256ByteSize + 1];
        printf("\n");
        for(int i = 0; i < mNbTop; i++)
        {
            toHex(hash, mTop[i]->hash);
            printf("|  %6d  |  %6" PRIu64 "  |  %s\n", i + 1, mTop[i]->height - 1, hash);
        }
        printf("\n");
    }

    virtual void startBlock(const Block *b, uint64_t chainSize)
    {
        mTop.push_back(b);
    }
};

static LowestHash lowestHash;
