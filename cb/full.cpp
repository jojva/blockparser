// Compute everything needed

#include <vector>
#include <util.h>
#include <common.h>
#include <option.h>
#include <callback.h>

struct Full:public Callback
{
    optparse::OptionParser parser;
    std::vector<Callback*> mCbs;

    Full()
    : mCbs()
    {
        parser
            .usage("")
            .version("")
            .description("Compute every callback needed.")
            .epilog("")
        ;
        parser
            .add_option("-l", "--low")
            .action("store_true")
            .set_default(false)
            .help("Enable lowestHash")
        ;
    }

    virtual const char                   *name() const         { return "full";        }
    virtual const optparse::OptionParser *optionParser() const { return &parser;       }
    virtual bool                         needTXHash() const    { return false;         }

    virtual int init(
        int  argc,
        const char *argv[]
    )
    {
        optparse::Values &values = parser.parse_args(argc, argv);
        auto args = parser.args();
        if(args.size() > 1)
        {
            // Load selectively
            if(values.get("low"))
            {
                mCbs.push_back(find("low"));
            }
            // Add at will
            // if(values.get("another"))
            // {
            //     mCbs.push_back(find("another"));
            // }
        }
        else
        {
            // Load everything
            mCbs.push_back(find("low"));
            // Add at will
            // mCbs.push_back(find("another"));
        }
        return 0;
    }

    virtual void     startMap(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startMap(p);}
    virtual void       endMap(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endMap(p);}
    virtual void   startBlock(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startBlock(p);}
    virtual void     endBlock(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endBlock(p);}
    virtual void        start(const Block *s, const Block *e       )       {for(auto cb : mCbs) cb->start(s, e);}
    virtual void     startTXs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startTXs(p);}
    virtual void       endTXs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endTXs(p);}
    virtual void      startTX(const uint8_t *p, const uint8_t *hash)       {for(auto cb : mCbs) cb->startTX(p, hash);}
    virtual void        endTX(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endTX(p);}
    virtual void  startInputs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startInputs(p);}
    virtual void    endInputs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endInputs(p);}
    virtual void   startInput(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startInput(p);}
    virtual void     endInput(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endInput(p);}
    virtual void startOutputs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startOutputs(p);}
    virtual void   endOutputs(const uint8_t *p                     )       {for(auto cb : mCbs) cb->endOutputs(p);}
    virtual void  startOutput(const uint8_t *p                     )       {for(auto cb : mCbs) cb->startOutput(p);}
    virtual void   startBlock(const Block *b, uint64_t chainSize   )       {for(auto cb : mCbs) cb->startBlock(b, chainSize);}
    virtual void     endBlock(const Block *b                       )       {for(auto cb : mCbs) cb->endBlock(b);}
    virtual void      startLC(                                     )       {for(auto cb : mCbs) cb->startLC();}
    virtual void       wrapup(                                     )       {for(auto cb : mCbs) cb->wrapup();}

    virtual void    endOutput(
        const uint8_t *p,
        uint64_t      value,
        const uint8_t *txHash,
        uint64_t      outputIndex,
        const uint8_t *outputScript,
        uint64_t      outputScriptSize
    )
    {
        for(auto cb : mCbs) cb->endOutput(p, value, txHash, outputIndex, outputScript, outputScriptSize);
    }

    virtual void edge(
        uint64_t      value,
        const uint8_t *upTXHash,
        uint64_t      outputIndex,
        const uint8_t *outputScript,
        uint64_t      outputScriptSize,
        const uint8_t *downTXHash,
        uint64_t      inputIndex,
        const uint8_t *inputScript,
        uint64_t      inputScriptSize
    )
    {
        for(auto cb : mCbs) cb->edge(value,
                                     upTXHash, outputIndex, outputScript, outputScriptSize,
                                     downTXHash, inputIndex, inputScript, inputScriptSize
                                    );
    }
};

static Full full;
