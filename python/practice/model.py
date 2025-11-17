from sqlalchemy import Column, String, Integer
from db import Base

class FruitWorld(Base):
    __tablename__ = "products"
    __table_args__ = ( {'schema' : 'public'} )
    food = Column(String(50), primary_key=True, nullable=False, unique=True)
    img = Column(String(50), nullable=False)