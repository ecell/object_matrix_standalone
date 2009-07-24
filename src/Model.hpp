#ifndef MODEL_HPP
#define MODEL_HPP

#include <boost/noncopyable.hpp>

#include "SerialIDGenerator.hpp"
#include "SpeciesTypeID.hpp"
#include "NetworkRules.hpp"

class SpeciesType;

class Model: private boost::noncopyable
{
public:
    typedef SpeciesTypeID species_id_type;
    typedef SpeciesType species_type_type;

private:
    typedef SerialIDGenerator<species_id_type> species_type_id_generator_type;
    typedef std::map<species_id_type, species_type_type*> species_type_map_type;
    typedef select_second<species_type_map_type::value_type> second_selector_type;

public:
    typedef boost::transform_iterator<second_selector_type,
            species_type_map_type::const_iterator> species_type_iterator;
    typedef boost::iterator_range<species_type_iterator> species_type_range;
    typedef NetworkRules network_rules_type;

public:
    Model();

    ~Model();

    network_rules_type& network_rules() const
    {
        return *network_rules_;
    }

    species_type_type& new_species_type();

    species_type_type& get_species_type(species_id_type const& id) const;

    species_type_range get_species_types() const;

public:
    species_type_id_generator_type species_type_id_generator_;
    species_type_map_type species_type_map_;
    network_rules_type *network_rules_;
};


#endif /* MODEL_HPP */
