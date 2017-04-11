template<class ComponentType>
inline componentWrapper_t SystemBase::AddComponent( entityID_t entity )
{
	size_t componentHashCode = typeid( ComponentType ).hash_code();

	if ( !this->isComponentRegistered( componentHashCode ) )
	{
		this->registerComponent( componentHashCode );
		return this->AddComponent<ComponentType>( entity );
	}
	if ( this->isCurrentBlockOverloaded( componentHashCode ) )
		this->allocateNewBlock<ComponentType>();

	return this->addToBlock( entity, componentHashCode );
}

template<class ComponentType>
inline componentWrapper_t SystemBase::GetComponent( entityID_t entity )
{
	if ( entity == 0 || !this->isEntityInSystem( entity ) )
		return componentWrapper_t();
	size_t componentHashCode = typeid( ComponentType ).hash_code();

	if ( !this->isComponentRegistered( componentHashCode ) )
		return componentWrapper_t();

	auto componentBlockPosition = this->componentsBlocks.begin();
	for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
		if ( i->hashCode == componentHashCode )
			componentBlockPosition = i;

	for ( auto& component : componentBlockPosition->data )
		if ( component.ownerEntityID == entity )
			return component;

	return componentWrapper_t();
}

template<class ComponentType, typename ...Args>
void SystemBase::ForEach( std::function<void( componentWrapper_t&, Args... )> func, Args&&... args )
{
	if ( !func )
		return;

	size_t componentHashCode = typeid( ComponentType ).hash_code();
	if ( !this->isComponentRegistered( componentHashCode ) )
		return;

	for ( auto i = this->componentsBlocks.begin(), tooFar = this->componentsBlocks.end(); i != tooFar; i++ )
		if ( i->hashCode == componentHashCode )
			for ( auto& component : i->data )
				func( component, std::forward<Args>( args )... );
}

template<class ComponentType>
inline void SystemBase::allocateNewBlock()
{
	this->componentsBlocks.emplace_back();
	this->componentsBlocks.back().hashCode = typeid( ComponentType ).hash_code();
	this->componentsBlocks.back().ReserveComponents<ComponentType>( MAX_COMPONENT_BLOCK_SIZE );
}

