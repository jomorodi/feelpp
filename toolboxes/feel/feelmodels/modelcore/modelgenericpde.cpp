/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim:fenc=utf-8:ft=cpp:et:sw=4:ts=4:sts=4
 */

#include <feel/feelmodels/modelcore/modelgenericpde.hpp>


namespace Feel
{
namespace FeelModels
{

template <uint16_type Dim>
ModelGenericPDE<Dim>::ModelGenericPDE( /*std::string const& physic*/ )
    :
    super_type( "GenericPDE" )
{}

template <uint16_type Dim>
ModelGenericPDE<Dim>::ModelGenericPDE( std::string const& name, pt::ptree const& p )
    :
    super_type( "GenericPDE" )
{
    this->setupGenericPDE( name, p );
}

template <uint16_type Dim>
void
ModelGenericPDE<Dim>::setupGenericPDE( std::string const& name, pt::ptree const& eqPTree )
{
    this->M_physicDefault = name;

    if ( auto nameEqOpt =  eqPTree.template get_optional<std::string>( "name" ) )
        this->M_physicDefault = *nameEqOpt;

    auto mphysic = std::make_shared<ModelPhysic<Dim>>( this->physicType(),  this->physicDefault() );

    if ( auto unknownPTree = eqPTree.get_child_optional("unknown") )
    {
        if ( auto unknownNameOpt =  unknownPTree->template get_optional<std::string>( "name" ) )
            M_unknownName = *unknownNameOpt;
        else
            CHECK( false ) << "require to define unknown.name";
        if ( auto unknownSymbolOpt =  unknownPTree->template get_optional<std::string>( "symbol" ) )
            M_unknownSymbol = *unknownSymbolOpt;
        else
            M_unknownSymbol = M_unknownName;
        if ( auto unknownBasisOpt =  unknownPTree->template get_optional<std::string>( "basis" ) )
            M_unknownBasis = *unknownBasisOpt;
        else
            M_unknownBasis = "Pch1";
    }

    std::string unknownShape;
    if ( M_unknownBasis == "Pch1" ||  M_unknownBasis == "Pch2" )
        unknownShape = "scalar";
    else if ( M_unknownBasis == "Pchv1" ||  M_unknownBasis == "Pchv2" )
        unknownShape = "vectorial";
    else
        CHECK( false ) << "invalid unknown.basis : " << M_unknownBasis;

    material_property_shape_dim_type scalarShape = std::make_pair(1,1);
    material_property_shape_dim_type vectorialShape = std::make_pair(nDim,1);
    material_property_shape_dim_type matrixShape = std::make_pair(nDim,nDim);

    mphysic->addMaterialPropertyDescription( this->convectionCoefficientName(), this->convectionCoefficientName(), { vectorialShape } );
    mphysic->addMaterialPropertyDescription( this->diffusionCoefficientName(), this->diffusionCoefficientName(), { scalarShape, matrixShape } );
    mphysic->addMaterialPropertyDescription( this->reactionCoefficientName(), this->reactionCoefficientName(), { scalarShape } );
    mphysic->addMaterialPropertyDescription( this->firstTimeDerivativeCoefficientName(), this->firstTimeDerivativeCoefficientName(), { scalarShape } );
    mphysic->addMaterialPropertyDescription( this->secondTimeDerivativeCoefficientName(), this->secondTimeDerivativeCoefficientName(), { scalarShape } );
    if ( unknownShape == "scalar" )
        mphysic->addMaterialPropertyDescription( this->sourceCoefficientName(), this->sourceCoefficientName(), { scalarShape } );
    else if ( unknownShape == "vectorial" )
        mphysic->addMaterialPropertyDescription( this->sourceCoefficientName(), this->sourceCoefficientName(), { vectorialShape } );

    this->M_physics.emplace( mphysic->name(), mphysic );
}

template <uint16_type Dim>
ModelGenericPDEs<Dim>::ModelGenericPDEs( /*std::string const& physic*/ )
    :
    super_type( "GenericPDEs" )
{}

template <uint16_type Dim>
void
ModelGenericPDEs<Dim>::setupGenericPDEs( std::string const& name, pt::ptree const& modelPTree )
{
    this->M_physicDefault = name;
    if ( auto equationsOpt = modelPTree.get_child_optional("equations") )
    {
        if ( equationsOpt->empty() )
        {
            std::string equationName = equationsOpt->get_value<std::string>();
            CHECK( false ) << "TODO";
        }
        else
        {
            for ( auto const& itemEq : *equationsOpt )
            {
                CHECK( itemEq.first.empty() ) << "should be an array, not a subtree";

                std::string nameEqDefault = (boost::format("equation%1%")%M_pdes.size()).str();
                ModelGenericPDE<nDim> mgpde( nameEqDefault, itemEq.second );
                M_pdes.push_back( std::move( mgpde ) );
            }
        }
    }

    auto mphysic = std::make_shared<ModelPhysic<Dim>>( this->physicType(), name );
    for ( auto const& pde : M_pdes )
    {
        this->M_physics.insert( pde.physics().begin(), pde.physics().end() );
        for ( auto const& subPhysic : pde.physics() ) // normally only one
            mphysic->addSubphysic( subPhysic.second );
    }
    this->M_physics.emplace( name, mphysic );
}

template class ModelGenericPDE<2>;
template class ModelGenericPDE<3>;
template class ModelGenericPDEs<2>;
template class ModelGenericPDEs<3>;

} // namespace FeelModels
} // namespace Feel