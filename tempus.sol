// SPDX-License-Identifier: MIT
pragma solidity ^0.8.4;

import "erc721a/contracts/ERC721A.sol";
import "@openzeppelin/contracts/access/Ownable.sol";

contract tempus is ERC721A, Ownable {
    uint256 public maxMintAmount = 4;
    uint256 public nftPerAddressLimit = 40;
    uint256 MAX_SUPPLY = 10000;
    uint256 public mintRate = 0.05 ether;
    bool public paused = true; 
    bool public revealed = false; 
    string private baseURI; //ipfs://[FILL THIS OUT WITH CID]/ , don't forget trailing /
    string public hiddenURI;

    constructor(string memory _initBaseURI, string memory _initHiddenURI) ERC721A("Tempus", "TEMPUS") {
        setBaseURI(_initBaseURI);
        setHiddenURI(_initHiddenURI);
    }

    //override startTokenID to start at 1 instead of 0
    function _startTokenId() internal view virtual override returns (uint256) {
        return 1;
    }

    function mint(uint256 quantity) external payable {
        // _safeMint's second argument now takes in a quantity, not a tokenId.
        require(totalSupply() + quantity <= MAX_SUPPLY, "Not enough NFTs left");

        if (msg.sender != owner()) {
            require(!paused, "the contract is paused");
            require(quantity <= maxMintAmount, "Exceeded the mint limit for this session");
            require(_numberMinted(msg.sender) + quantity <= nftPerAddressLimit, "max NFT per address exceeded");
            require(msg.value >= (mintRate * quantity), "Not enough ether sent");
        }

        _safeMint(msg.sender, quantity);
    }

    //AMOUNT MEASURED IN WEI
    //note for next time: no reason to make this a payable fuction
    function withdraw(uint256 amount) external payable onlyOwner {
        (bool success, ) = payable(owner()).call{value: amount}("");
        require(success);
    }

    function _baseURI() internal view override returns (string memory) {
        return baseURI;
    }

    function setMintRate(uint256 _mintRate) public onlyOwner {
        mintRate = _mintRate;
    }

    function setmaxMintAmount(uint256 _newmaxMintAmount) public onlyOwner {
        maxMintAmount = _newmaxMintAmount;
    }

    function toggleReveal(bool _revealed) public onlyOwner {
      revealed = _revealed;
    }

    function setNftPerAddressLimit(uint256 _limit) public onlyOwner {
        nftPerAddressLimit = _limit;
    }
  
    function setBaseURI(string memory _newBaseURI) public onlyOwner {
        baseURI = _newBaseURI;
    }

    function setHiddenURI(string memory _hiddenURI) public onlyOwner {
        hiddenURI = _hiddenURI;
    }

    function pause(bool _state) public onlyOwner {
        paused = _state;
    }

    function tokenURI(uint256 tokenId) public view override returns (string memory) {
        if (!_exists(tokenId)) revert URIQueryForNonexistentToken();

        if (revealed == false) {
            return hiddenURI;
        }

        string memory currentBaseURI = _baseURI();
        return bytes(currentBaseURI).length != 0 ? string(abi.encodePacked(currentBaseURI, Strings.toString(tokenId), ".json")) : "";
    }

    function numberMinted(address owner) public view returns (uint256) {
        return _numberMinted(owner);
    }

    function getOwnershipData(uint256 tokenId) external view returns (TokenOwnership memory) {
        return _ownershipOf(tokenId);
    }
}