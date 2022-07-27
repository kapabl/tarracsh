#ifndef SIGNATURE_KLEENE_RULE_H
#define SIGNATURE_KLEENE_RULE_H

#include "Rule.h"
#include "NodeTraits.h"
#include "../SignatureScanner.h"


namespace org::kapa::tarracsh::signatures {

// class Kleene;

// template <typename T>
// bool invokeMatch(Rule::RuleVariant &ruleVariant, SignatureScanner &scanner, T &node);


class Kleene final : public Rule {
public:
    Kleene();
    Kleene(RuleVariant rule, int minimum);

    Kleene(const Kleene &other);

    Kleene(Kleene &&other) noexcept;

    Kleene &operator=(const Kleene &other);

    Kleene &operator=(Kleene &&other) noexcept;

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        bool result = false;
        DEBUG_RULE(this);

        result = MatchSelector<is_list<T>::value, T>::match(*this, scanner, node);

        return result;
    }

    ~Kleene() override = default;


private:
    //Rule _ruleVariant;
    RuleVariant _ruleVariant;
    int _minimum = 0;

    template<bool IsList, typename TNode>
    struct MatchSelector {
        static bool match(Kleene& rule, SignatureScanner& scanner, TNode& node) {
            const bool result = matchNode<visit_struct::traits::is_visitable<TNode>::value>(rule, scanner, node);
            return result;
        }

        template <bool IsVisitable>
        static bool matchNode(Kleene& rule, SignatureScanner& scanner, TNode& node ) {
            const bool result = rule.matchToNonVisitable(scanner, node);
            return result;
        }


        template <>
        static bool matchNode<true>(Kleene& rule, SignatureScanner& scanner, TNode& node) {
            const bool result = rule.matchToVisitable(scanner, node);
            return result;
        }
    };

    template< typename TNode>
    struct MatchSelector<true, TNode> {
        static bool match(Kleene& rule, SignatureScanner& scanner, TNode& node) {
            const bool result = rule.matchToList(scanner, node);
            return result;
        }
    };

    template <typename T>
    bool matchToList(SignatureScanner& scanner, std::vector<T>& list) {
        bool result = false;


        const auto startPosition = scanner.getPosition();
        auto matchedCount = 0;
        T item{};
        // while (_ruleVariant.match(scanner, item)) {
        auto matchResult = invokeMatch(_ruleVariant, scanner, item);
        while (matchResult.first) {
            list.push_back(item);
            matchedCount++;
            matchResult = invokeMatch(_ruleVariant, scanner, item);
        }

        result = matchedCount >= _minimum;
        if (!result) {
            scanner.reset(startPosition);
        }

        //TODO match the followBy
        return result;
    }


    template <typename T>
    bool matchToVisitable(SignatureScanner& scanner, T& node) {
        bool result = false;

        auto childNode = visit_struct::get<0>(node);

        typedef visit_struct::type_at<0, T> ChildNodeType;

        result = MatchSelector<is_list<ChildNodeType>::value, ChildNodeType>::match(*this, scanner, childNode);
       
        return result;
    }


    template <typename T>
    bool matchToNonVisitable(SignatureScanner& scanner, T& node) {
        bool result = false;

        //TODO this should ber a terminal type, what is this doing here?
        return true;
    }

};

}


#endif
